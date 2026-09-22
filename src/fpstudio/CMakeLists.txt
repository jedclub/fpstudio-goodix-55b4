cmake_minimum_required(VERSION 3.21)
project(fpstudio VERSION 0.3.0 LANGUAGES C CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

# CMake's Release profile supplies -O3; IPO adds LTO where the compiler
# supports it. The CPU-side hot path is deliberately kept small because the
# expensive comparison search is dispatched to the persistent Vulkan worker.
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
add_compile_options(-Wall -Wextra -Wno-unused-parameter)

# Profile-guided optimisation.
#
# Two configures of the same tree: one that records which branches are taken,
# and one that compiles with that record. The training run in between is what
# makes it worth anything - see tools/pgo-build.sh, which uses the test suite
# and a synthetic matcher sweep so that training needs no fingerprint data and
# can therefore run anywhere, including CI.
#
#   cmake -DFPSTUDIO_PGO=GENERATE ..   # then build, then run the training
#   cmake -DFPSTUDIO_PGO=USE ..        # then build again
#
# The profile lives outside the build directory on purpose: the USE configure
# wipes and rewrites object files, and a profile stored among them would be
# destroyed by the build that is supposed to consume it.
set(FPSTUDIO_PGO OFF CACHE STRING "Profile-guided optimisation: OFF, GENERATE or USE")
set_property(CACHE FPSTUDIO_PGO PROPERTY STRINGS OFF GENERATE USE)
set(FPSTUDIO_PGO_DIR "${CMAKE_CURRENT_BINARY_DIR}/../pgo-data"
    CACHE PATH "Where -fprofile-generate writes and -fprofile-use reads")
get_filename_component(FPSTUDIO_PGO_DIR "${FPSTUDIO_PGO_DIR}" ABSOLUTE)

if(FPSTUDIO_PGO STREQUAL "GENERATE")
  file(MAKE_DIRECTORY "${FPSTUDIO_PGO_DIR}")
  # prefer-atomic: the preview reader, the Vulkan worker and Qt's own pools all
  # run counters concurrently, and the default non-atomic update loses them.
  add_compile_options(-fprofile-generate "-fprofile-dir=${FPSTUDIO_PGO_DIR}"
                      -fprofile-update=prefer-atomic)
  add_link_options(-fprofile-generate "-fprofile-dir=${FPSTUDIO_PGO_DIR}")
  message(STATUS "PGO: recording to ${FPSTUDIO_PGO_DIR}")
elseif(FPSTUDIO_PGO STREQUAL "USE")
  if(NOT EXISTS "${FPSTUDIO_PGO_DIR}")
    message(FATAL_ERROR "FPSTUDIO_PGO=USE but no profile at ${FPSTUDIO_PGO_DIR}; run the GENERATE build and its training first")
  endif()
  # A source file the training never reached has no profile, which is normal
  # for error paths and must not fail the build. Correction is needed because
  # the atomic counters above can still be raced across processes.
  add_compile_options(-fprofile-use "-fprofile-dir=${FPSTUDIO_PGO_DIR}"
                      -fprofile-correction -Wno-missing-profile)
  add_link_options(-fprofile-use "-fprofile-dir=${FPSTUDIO_PGO_DIR}")
  message(STATUS "PGO: compiling against ${FPSTUDIO_PGO_DIR}")
elseif(NOT FPSTUDIO_PGO STREQUAL "OFF")
  message(FATAL_ERROR "FPSTUDIO_PGO must be OFF, GENERATE or USE (got '${FPSTUDIO_PGO}')")
endif()

# Authentication logs may include __FILE__ through libsystemd's journal
# macros.  Strip the build machine's absolute checkout path from every target
# so Release binaries are reproducible and never disclose a user/home name.
get_filename_component(FPSTUDIO_REPOSITORY_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
add_compile_options(
  "-ffile-prefix-map=${FPSTUDIO_REPOSITORY_ROOT}=."
  "-fmacro-prefix-map=${FPSTUDIO_REPOSITORY_ROOT}=."
  "-fdebug-prefix-map=${FPSTUDIO_REPOSITORY_ROOT}=."
)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets Concurrent LinguistTools)
find_package(PkgConfig REQUIRED)
pkg_check_modules(FPRINT REQUIRED IMPORTED_TARGET libfprint-2)
pkg_check_modules(GLIB   REQUIRED IMPORTED_TARGET glib-2.0 gobject-2.0)

qt_standard_project_setup()
# Named timing scopes in the matcher's CPU paths. Off by default and compiled
# out entirely when off - see tools/vkmatch/profile.h for why that matters.
option(FPSTUDIO_PROFILE "Build the matcher with timing scopes enabled" OFF)
if(FPSTUDIO_PROFILE)
  add_compile_definitions(FPSTUDIO_PROFILE=1)
  message(STATUS "Matcher timing scopes: enabled (do not ship this build)")
endif()

include(CTest)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../../tools/vkmatch" "${CMAKE_BINARY_DIR}/vkmatch")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../pam" "${CMAKE_BINARY_DIR}/pam")

qt_add_executable(fpstudio
  src/main.cpp
  src/engine.cpp
  src/headless.cpp
  src/mainwindow.cpp
  src/privrunner.cpp
  src/beacon.cpp
  src/i18n.cpp
  src/setupcheck.cpp
  src/setupwizard.cpp
  src/researchwindow.cpp
  src/livewindow.cpp
)
add_dependencies(fpstudio fpstudio-vkmatch)

# The eleven languages the tool ships. English is the source language and has
# no catalogue of its own - see i18n.cpp.
set(FPSTUDIO_LANGUAGES ko ja zh_CN zh_TW es de fr ru it pt)

# Translations are compiled into the build tree and installed beside the
# binary. i18n.cpp looks in both, so running from a build directory works
# without installing - which is what anyone working on a translation does.
set(FPSTUDIO_TS_FILES "")
foreach(lang ${FPSTUDIO_LANGUAGES})
  list(APPEND FPSTUDIO_TS_FILES ${CMAKE_SOURCE_DIR}/translations/fpstudio_${lang}.ts)
endforeach()

qt_add_translations(fpstudio
  TS_FILES ${FPSTUDIO_TS_FILES}
  QM_FILES_OUTPUT_VARIABLE FPSTUDIO_QM_FILES
  LUPDATE_OPTIONS -no-obsolete
)

# Where a build tree keeps its .qm files, so the binary can find them before
# being installed. Quoted through to C++ as a string literal.
# The installed fp-image.h does `#include "sigfm/sigfm.hpp"`, but the fork's
# package does not ship that header - a packaging gap, not something wrong
# with our build. It is a dependency-free declaration block (typedefs plus an
# extern "C" block), so a vendored copy taken from the matching source tree
# satisfies the include without pulling in opencv.
target_include_directories(fpstudio PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty)

target_link_libraries(fpstudio PRIVATE
  Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Concurrent
  PkgConfig::FPRINT
  PkgConfig::GLIB
  vkmatch
)
add_dependencies(fpstudio mapplace-shader)
install(FILES "${CMAKE_BINARY_DIR}/vkmatch/fpstudio-mapplace.spv"
        DESTINATION ${CMAKE_INSTALL_DATADIR}/fpstudio)

install(TARGETS fpstudio RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${FPSTUDIO_QM_FILES} DESTINATION ${CMAKE_INSTALL_DATADIR}/fpstudio/translations)

# Keep the setup wizard useful after installation.  The helper scripts derive
# their project root from their own location, so preserve the small subset of
# the repository layout they need under one versioned, read-only data tree.
set(FPSTUDIO_SETUP_DIR ${CMAKE_INSTALL_DATADIR}/fpstudio/setup)
install(PROGRAMS
  ../../tools/driver_build.py
  ../../tools/auth_install.py
  ../../tools/dual_auth_install.py
  ../../tools/admin_notice.py
  ../../tools/auth_log_report.py
  DESTINATION ${FPSTUDIO_SETUP_DIR}/tools)
# auth_install.py resolves this relative to the setup tree, so the drop-in has
# to sit at the same path under it that it occupies in the checkout.
install(FILES systemd/61-fpstudio-guard.conf
        DESTINATION ${FPSTUDIO_SETUP_DIR}/src/fpstudio/systemd)
install(PROGRAMS fprintd-beacon.sh DESTINATION ${FPSTUDIO_SETUP_DIR}/src/fpstudio)
install(FILES 99-fpstudio-goodix.rules DESTINATION ${FPSTUDIO_SETUP_DIR}/src/fpstudio)
install(FILES pam/admin-auth-notice.txt
        DESTINATION ${FPSTUDIO_SETUP_DIR}/src/fpstudio/pam)
install(FILES ../driver/PKGBUILD DESTINATION ${FPSTUDIO_SETUP_DIR}/src/driver)
install(DIRECTORY ../driver/patches/ DESTINATION ${FPSTUDIO_SETUP_DIR}/src/driver/patches
        FILES_MATCHING PATTERN "*.patch")
install(FILES
  ../../README.md
  ../../LICENSE
  ../../THIRD_PARTY_NOTICES.md
  ../../docs/00-setup.md
  ../../docs/21-simultaneous-auth.md
  ../../docs/releases/v0.3.0.md
  ../../docs/23-fprintd-runaway.md
  ../../docs/24-optimisation.md
  DESTINATION ${CMAKE_INSTALL_DATADIR}/doc/fpstudio)

include(CTest)
if(BUILD_TESTING)
  add_executable(fpstudio-setup-policy-test tests/setup_policy_test.cpp src/setupcheck.cpp)
  target_include_directories(fpstudio-setup-policy-test PRIVATE src)
  target_link_libraries(fpstudio-setup-policy-test PRIVATE Qt6::Core)
  add_test(NAME setup-policy-safety COMMAND fpstudio-setup-policy-test)
  find_package(Qt6 REQUIRED COMPONENTS Test)
  qt_add_executable(fpstudio-fake-capture tests/fakecapture.cpp)
  target_link_libraries(fpstudio-fake-capture PRIVATE Qt6::Core Qt6::Gui)
  qt_add_executable(fpstudio-research-test tests/research_test.cpp src/researchwindow.cpp src/livewindow.cpp src/beacon.cpp)
  target_include_directories(fpstudio-research-test PRIVATE src)
  target_compile_definitions(fpstudio-research-test PRIVATE FAKE_CAPTURE="$<TARGET_FILE:fpstudio-fake-capture>" FPSTUDIO_APP="$<TARGET_FILE:fpstudio>")
  target_link_libraries(fpstudio-research-test PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Test vkmatch)
  add_dependencies(fpstudio-research-test mapplace-shader)
  add_dependencies(fpstudio-research-test fpstudio-fake-capture fpstudio)
  add_test(NAME research-workflow COMMAND fpstudio-research-test)
  set_tests_properties(research-workflow PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")

  # Guards the window geometry itself: that neither window's minimum size can
  # leave the screen, and that no caption can grow it off the bottom. See the
  # comment at the head of layout_test.cpp for what this is protecting against.
  qt_add_executable(fpstudio-layout-test tests/layout_test.cpp src/livewindow.cpp src/beacon.cpp)
  target_include_directories(fpstudio-layout-test PRIVATE src)
  target_link_libraries(fpstudio-layout-test PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Test vkmatch)
  add_dependencies(fpstudio-layout-test mapplace-shader)
  add_test(NAME window-layout COMMAND fpstudio-layout-test)
  set_tests_properties(window-layout PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
endif()
