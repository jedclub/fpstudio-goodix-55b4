cmake_minimum_required(VERSION 3.21)
project(fpstudio VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

# -O2 plus LTO. The hot path is decoding an 88x108 frame and scaling it for
# display, which is small; the reason to optimise is latency during a capture,
# not throughput.
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
add_compile_options(-Wall -Wextra -Wno-unused-parameter)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets LinguistTools)
find_package(PkgConfig REQUIRED)
pkg_check_modules(FPRINT REQUIRED IMPORTED_TARGET libfprint-2)
pkg_check_modules(GLIB   REQUIRED IMPORTED_TARGET glib-2.0 gobject-2.0)

qt_standard_project_setup()

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
)

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
target_compile_definitions(fpstudio PRIVATE
  FPSTUDIO_TRANSLATIONS_DIR="${CMAKE_BINARY_DIR}"
  # The setup checks offer to install files that live in this tree - the udev
  # rule, the PAM stack, the driver PKGBUILD. A build tree finds them relative
  # to the binary; this covers the case where it cannot.
  FPSTUDIO_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

# The installed fp-image.h does `#include "sigfm/sigfm.hpp"`, but the fork's
# package does not ship that header - a packaging gap, not something wrong
# with our build. It is a dependency-free declaration block (typedefs plus an
# extern "C" block), so a vendored copy taken from the matching source tree
# satisfies the include without pulling in opencv.
target_include_directories(fpstudio PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty)

target_link_libraries(fpstudio PRIVATE
  Qt6::Core Qt6::Gui Qt6::Widgets
  PkgConfig::FPRINT
  PkgConfig::GLIB
)

install(TARGETS fpstudio RUNTIME DESTINATION bin)
install(FILES ${FPSTUDIO_QM_FILES} DESTINATION share/fpstudio/translations)
