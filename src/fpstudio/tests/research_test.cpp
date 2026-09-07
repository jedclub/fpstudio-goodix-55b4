#include "researchwindow.h"
#include "livewindow.h"
#include "frameselector.h"
#include <QLabel>
#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPushButton>
#include <QTemporaryDir>
#include <QTest>
#include <cmath>
#include <QCryptographicHash>
#include "../../driver/experimental/preview_state.h"

class ResearchTest : public QObject {
    Q_OBJECT
    QJsonObject status(const QString &dir) {
        QFile file(dir + "/session.json");
        if (!file.open(QIODevice::ReadOnly)) return {};
        return QJsonDocument::fromJson(file.readAll()).object();
    }
private slots:
    void targetStopsWithoutExtraUserAction() {
        QTemporaryDir dir,refs;
        qputenv("FPSTUDIO_TEST_CONTACTS","33");
        fpstudio::LiveWindow w(dir.path(),nullptr,FAKE_CAPTURE,1,refs.path(),false,FAKE_CAPTURE,{},3);
        QTest::qWait(100);qunsetenv("FPSTUDIO_TEST_CONTACTS");
        auto read=[&]{QFile f(dir.path()+"/live-status.json");if(!f.open(QIODevice::ReadOnly))return QJsonObject();return QJsonDocument::fromJson(f.readAll()).object();};
        QTRY_COMPARE_WITH_TIMEOUT(read().value("state").toString(),"ended",7000);
        QVERIFY(read().value("target_reached").toBool());
        QCOMPARE(read().value("compared_contacts").toInt(),3);
        QCOMPARE(read().value("pending_contacts").toInt(),0);
        QVERIFY(read().value("instruction").toString().contains(QStringLiteral("더 반복하지 마세요")));
        QTest::qWait(500); // Include the asynchronous QProcess termination events.
        QCOMPARE(read().value("gpu_errors").toInt(),0);
        QVERIFY(read().value("gpu_message").toString().contains(QStringLiteral("정상 종료")));
        QVERIFY(!read().value("gpu_message").toString().contains(QStringLiteral("실행할 수 없습니다")));
        for(const auto *label:w.findChildren<QLabel *>())
            QVERIFY(!label->text().contains(QStringLiteral("실행할 수 없습니다")));
    }
    void cascadeFallbackAndBankIntegrity() {
        QTemporaryDir dir;
        QImage ref(108,88,QImage::Format_Grayscale8),flat(108,88,QImage::Format_Grayscale8);
        flat.fill(128);
        for(int y=0;y<88;++y)for(int x=0;x<108;++x)ref.scanLine(y)[x]=int(127+45*std::sin(.21*x+.17*y)+33*std::cos(.07*x-.43*y));
        QVERIFY(ref.save(dir.path()+"/reference-index-01.png"));
        QVERIFY(flat.save(dir.path()+"/reference-index-02.png"));
        QVERIFY(ref.save(dir.path()+"/probe.png"));
        QFile pixels(dir.path()+"/reference-index-02.png");QVERIFY(pixels.open(QIODevice::ReadOnly));
        auto hash=QString::fromLatin1(QCryptographicHash::hash(pixels.readAll(),QCryptographicHash::Sha256).toHex());
        auto manifest=[&](const QString &digest){QFile f(dir.path()+"/manifest.json");if(!f.open(QIODevice::WriteOnly))return false;
            f.write(QJsonDocument(QJsonObject{{"schema_version",1},{"research_only",true},{"references",QJsonArray{
                QJsonObject{{"file","reference-index-02.png"},{"sha256",digest}}}}}).toJson());return true;};
        QVERIFY(manifest(hash));
        QStringList args{"--reference-dir",dir.path(),"--fast-reference-bank",dir.path(),"--probe",dir.path()+"/probe.png"};
        QProcess process;process.start(QCoreApplication::applicationDirPath()+"/fpstudio-vkmatch",args);
        QVERIFY(process.waitForFinished(10000));QCOMPARE(process.exitCode(),0);
        const auto result=QJsonDocument::fromJson(process.readAllStandardOutput()).object();
        QCOMPARE(result.value("algorithm_version").toInt(),5);
        QVERIFY(result.value("full_bank_fallback").toBool());
        QVERIFY(result.value("best").toObject().value("interior").toObject().value("consistent").toBool());
        QVERIFY(result.value("authentication_decision").isNull());
        QVERIFY(manifest(QString(64,'0')));
        process.start(QCoreApplication::applicationDirPath()+"/fpstudio-vkmatch",args);
        QVERIFY(process.waitForFinished(10000));QVERIFY(process.exitCode()!=0);
    }
    void previewCalibrationRejectsCommonModeDrift() {
        GoodixPreviewState state{};uint16_t raw[GP_PIXELS];
        auto frame=[&](int offset,bool finger){
            for(int i=0;i<GP_PIXELS;++i) {
                raw[i]=2000+i%11+offset+(finger?int(180*std::sin((i%108)*.7)):0);
            }
            gp_update(&state,raw);
        };
        for(int i=0;i<59;++i)frame(0,false);
        QVERIFY(!state.ready);frame(0,false);QVERIFY(state.ready);QVERIFY(!state.touching);
        // Reproduce a background difference of 200+ without a fingerprint.
        for(int i=0;i<20;++i)frame(200+i,false);
        QVERIFY(state.raw_diff>200);QVERIFY(state.signal<1);QVERIFY(!state.touching);
        for(int touch=0;touch<10;++touch) {
            frame(220,true);frame(220,true);QVERIFY(!state.touching);
            frame(220,true);QVERIFY(state.touching);
            frame(220,false);frame(220,false);QVERIFY(state.touching);
            frame(220,false);QVERIFY(!state.touching);
        }
        QCOMPARE(state.background[0],2000.0); // Never learn a finger as background.
    }
    void failedContactRetriesDifferentFrames() {
        QTemporaryDir dir,refs;
        qputenv("FPSTUDIO_TEST_RETRY","1");
        fpstudio::LiveWindow w(dir.path(),nullptr,FAKE_CAPTURE,1,refs.path(),false,FAKE_CAPTURE);
        QTest::qWait(100);qunsetenv("FPSTUDIO_TEST_RETRY");
        auto read=[&]{QFile f(dir.path()+"/live-status.json");if(!f.open(QIODevice::ReadOnly))return QJsonObject();return QJsonDocument::fromJson(f.readAll()).object();};
        QTRY_COMPARE_WITH_TIMEOUT(read().value("state").toString(),"ended",10000);
        QCOMPARE(read().value("touches").toInt(),1);
        QCOMPARE(read().value("gpu_requests").toInt(),3);
        QCOMPARE(read().value("compared_contacts").toInt(),1);
        QCOMPARE(read().value("consistent_contacts").toInt(),0);
        QFile f(dir.path()+"/gpu-match-0003.json");QVERIFY(f.open(QIODevice::ReadOnly));
        QCOMPARE(QJsonDocument::fromJson(f.readAll()).object().value("selection_kind").toString(),"diverse-retry");
    }
    void stopCancelsBacklog() {
        QTemporaryDir dir,refs;
        qputenv("FPSTUDIO_TEST_CONTACTS","33");
        fpstudio::LiveWindow w(dir.path(),nullptr,FAKE_CAPTURE,1,refs.path(),false,FAKE_CAPTURE);
        QTest::qWait(100);qunsetenv("FPSTUDIO_TEST_CONTACTS");
        auto read=[&]{QFile f(dir.path()+"/live-status.json");if(!f.open(QIODevice::ReadOnly))return QJsonObject();return QJsonDocument::fromJson(f.readAll()).object();};
        QTRY_VERIFY_WITH_TIMEOUT(read().value("pending_contacts").toInt()>=2,3000);
        QTest::mouseClick(w.findChildren<QPushButton *>().first(),Qt::LeftButton);
        QTRY_COMPARE_WITH_TIMEOUT(read().value("state").toString(),"ended",2000);
        QCOMPARE(read().value("pending_contacts").toInt(),0);
        QVERIFY(!read().value("gpu_busy").toBool());
        bool cancelled=false;
        for(const auto &c:read().value("contacts").toArray())cancelled|=c.toObject().value("skip_reason").toString()=="user-stopped";
        QVERIFY(cancelled);
        QTest::qWait(500);
        QCOMPARE(read().value("gpu_errors").toInt(),0);
        QVERIFY(read().value("gpu_message").toString().contains(QStringLiteral("사용자 요청")));
        QVERIFY(!read().value("gpu_message").toString().contains(QStringLiteral("실행할 수 없습니다")));
    }
    void scaleRefinementAndNegativeControls() {
        QTemporaryDir dir;
        auto texture=[](double x,double y){return int(127+45*std::sin(.21*x+.17*y)+33*std::cos(.07*x-.43*y)+23*std::sin(.011*x*y+.4));};
        QImage ref(108,88,QImage::Format_Grayscale8);
        for(int y=0;y<88;++y)for(int x=0;x<108;++x)ref.scanLine(y)[x]=texture(x,y);
        QVERIFY(ref.save(dir.path()+"/reference.png"));
        for(double scale:{.875,1.125,0.,-1.}) {
            QImage probe(108,88,QImage::Format_Grayscale8);
            quint32 random=19391;
            for(int y=0;y<88;++y)for(int x=0;x<108;++x) {
                random=1664525u*random+1013904223u;
                probe.scanLine(y)[x]=scale>0?texture(53.5+(x-53.5)/scale,43.5+(y-43.5)/scale):scale==0?128:random>>24;
            }
            QVERIFY(probe.save(dir.path()+"/probe.png"));
            QProcess process;
            process.start(QCoreApplication::applicationDirPath()+"/fpstudio-vkmatch",{"--reference",dir.path()+"/reference.png","--probe",dir.path()+"/probe.png","--scale-refinement"});
            QVERIFY(process.waitForFinished(15000));QCOMPARE(process.exitCode(),0);
            const auto result=QJsonDocument::fromJson(process.readAllStandardOutput()).object();
            QCOMPARE(result.value("algorithm_version").toInt(),4);
            QVERIFY(result.value("authentication_decision").isNull());
            const auto best=result.value("best").toObject();
            if(scale>0) {
                QVERIFY2(best.value("interior").toObject().value("consistent").toBool(),QJsonDocument(result).toJson().constData());
                QVERIFY(std::abs(best.value("scale").toDouble()-scale)<.03);
                QVERIFY(best.value("ncc").toDouble()>.85);
            } else QVERIFY(!best.value("interior").toObject().value("consistent").toBool());
        }
    }
    void allContactsQueuedAndAccounted() {
        QTemporaryDir dir,refs;
        qputenv("FPSTUDIO_TEST_CONTACTS","33");
        qputenv("FPSTUDIO_TEST_EXTENDED","1");
        fpstudio::LiveWindow w(dir.path(),nullptr,FAKE_CAPTURE,1,refs.path(),false,FAKE_CAPTURE);
        // The child inherits this when start() is called on the event loop.
        QTest::qWait(100);
        qunsetenv("FPSTUDIO_TEST_CONTACTS");
        qunsetenv("FPSTUDIO_TEST_EXTENDED");
        auto read=[&]{QFile f(dir.path()+"/live-status.json");if(!f.open(QIODevice::ReadOnly))return QJsonObject();return QJsonDocument::fromJson(f.readAll()).object();};
        QTRY_COMPARE_WITH_TIMEOUT(read().value("state").toString(),"ended",25000);
        const auto result=read();
        QCOMPARE(result.value("touches").toInt(),33);
        QCOMPARE(result.value("candidates").toInt(),32);
        QCOMPARE(result.value("compared_contacts").toInt(),32);
        QCOMPARE(result.value("no_candidate_contacts").toInt(),1);
        QCOMPARE(result.value("uncompared_contacts").toInt(),1);
        QCOMPARE(result.value("pending_contacts").toInt(),0);
        QCOMPARE(result.value("gpu_requests").toInt(),32);
        QCOMPARE(result.value("gpu_completed").toInt(),32);
        QCOMPARE(result.value("gpu_errors").toInt(),0);
        QVERIFY(result.value("authentication_decision").isNull());
        const auto contacts=result.value("contacts").toArray();
        QCOMPARE(contacts.size(),33);
        QVERIFY(!contacts[0].toObject().value("candidate_saved").toBool());
        QCOMPARE(contacts[0].toObject().value("last_quality_reason").toString(),QStringLiteral("접촉 면적 부족"));
        for(int n=1;n<=32;++n) {
            QFile f(dir.path()+QString("/gpu-match-%1.json").arg(n,4,10,QLatin1Char('0')));
            QVERIFY(f.open(QIODevice::ReadOnly));
            const auto item=QJsonDocument::fromJson(f.readAll()).object();
            QCOMPARE(item.value("contact_group").toInt(),n+1);
            QVERIFY(item.value("sensor_timestamp_us").toInteger()>n*10);
            QVERIFY(!(f.permissions()&(QFileDevice::ReadGroup|QFileDevice::ReadOther)));
        }
        QVERIFY(QFile::exists(dir.path()+"/candidate-touch-33.png"));
        QCOMPARE(w.findChildren<QPushButton *>().size(),1);
        QCOMPARE(w.findChildren<QPushButton *>().first()->text(),QStringLiteral("닫기"));
    }
    void unavailableMatcherIsExplicit() {
        QTemporaryDir dir,refs;
        fpstudio::LiveWindow w(dir.path(),nullptr,FAKE_CAPTURE,1,refs.path(),false,dir.path()+"/missing-matcher");
        auto read=[&]{QFile f(dir.path()+"/live-status.json");if(!f.open(QIODevice::ReadOnly))return QJsonObject();return QJsonDocument::fromJson(f.readAll()).object();};
        QTRY_COMPARE_WITH_TIMEOUT(read().value("candidates").toInt(),1,3000);
        QCOMPARE(read().value("compared_contacts").toInt(),0);
        QCOMPARE(read().value("contacts").toArray()[0].toObject().value("skip_reason").toString(),"matcher-unavailable");
        QTest::mouseClick(w.findChildren<QPushButton *>().first(),Qt::LeftButton);
        QTRY_COMPARE_WITH_TIMEOUT(read().value("state").toString(),"ended",2000);
    }
    void fullRotationSearch() {
        QTemporaryDir dir;
        auto texture=[](double x,double y){return int(127+45*std::sin(.21*x+.17*y)+33*std::cos(.07*x-.43*y)+23*std::sin(.011*x*y+.4));};
        QImage ref(108,88,QImage::Format_Grayscale8);
        for(int y=0;y<88;++y)for(int x=0;x<108;++x)ref.scanLine(y)[x]=texture(x,y);
        QVERIFY(ref.save(dir.path()+"/reference.png"));
        for(int expected:{0,90,180,-175}) {
            QImage probe(108,88,QImage::Format_Grayscale8);
            const double a=expected*3.141592653589793/180,c=std::cos(a),s=std::sin(a);
            for(int y=0;y<88;++y)for(int x=0;x<108;++x) {
                const double dx=x-53.5,dy=y-43.5;
                probe.scanLine(y)[x]=texture(53.5+c*dx+s*dy,43.5-s*dx+c*dy);
            }
            QVERIFY(probe.save(dir.path()+"/probe.png"));
            QProcess process;
            process.start(QCoreApplication::applicationDirPath()+"/fpstudio-vkmatch",{"--reference",dir.path()+"/reference.png","--probe",dir.path()+"/probe.png"});
            QVERIFY(process.waitForFinished(15000));QCOMPARE(process.exitCode(),0);
            const auto result=QJsonDocument::fromJson(process.readAllStandardOutput()).object();
            QCOMPARE(result.value("algorithm_version").toInt(),3);
            QVERIFY(result.value("authentication_decision").isNull());
            const auto best=result.value("best").toObject();
            QVERIFY2(std::abs(std::remainder(best.value("angle").toDouble()-expected,360.))<=2,QJsonDocument(result).toJson().constData());
            QVERIFY(best.value("interior").toObject().value("consistent").toBool());
            QVERIFY(best.value("ncc").toDouble()>.85);
            if(expected!=0)QVERIFY(result.value("wide_rotation").toBool());
            else QVERIFY(!result.value("wide_rotation").toBool());
        }
    }
    void liveVulkanIntegration() {
        QTemporaryDir dir, refs;
        QImage pattern(108,88,QImage::Format_Grayscale8);
        for(int y=0;y<88;++y)for(int x=0;x<108;++x)pattern.scanLine(y)[x]=(x/3)%2?220:30;
        QVERIFY(pattern.save(refs.path()+"/reference-index-01.png"));
        fpstudio::LiveWindow w(dir.path(),nullptr,FAKE_CAPTURE,1,refs.path(),false);
        w.show();
        QCOMPARE(w.findChildren<QPushButton *>().size(),1);
        QTRY_VERIFY_WITH_TIMEOUT(QFile::exists(dir.path()+"/gpu-match-0001.json"),10000);
        QFile result(dir.path()+"/gpu-match-0001.json");QVERIFY(result.open(QIODevice::ReadOnly));
        const auto object=QJsonDocument::fromJson(result.readAll()).object();
        QVERIFY(object.value("ok").toBool());QVERIFY(object.value("research_only").toBool());
        QVERIFY(object.value("authentication_decision").isNull());
        QCOMPARE(object.value("algorithm_version").toInt(),8);
        QCOMPARE(object.value("auth_profile").toString(),QStringLiteral("auth-v8-contact-anchored-ridge-roi-uniform-scale-5pct"));
        QCOMPARE(object.value("instructed_role").toString(),"unspecified");
        QVERIFY(object.value("best").toObject().value("interior").isObject());
        QVERIFY(object.value("best").toObject().value("interior").toObject().value("supported_mask").toInt()>0);
        QVERIFY(QFile::exists(dir.path()+"/gpu-overlay-0001.png"));
        const QImage overlay(dir.path()+"/gpu-overlay-0001.png");QCOMPARE(overlay.size(),QSize(432,410));
        int green=0;
        for(int y=32;y<320;++y)for(int x=32;x<400;++x) {
            const QColor color=overlay.pixelColor(x,y);
            if(color.green()>color.red()+20)++green;
        }
        QVERIFY(green>100);
        QVERIFY(object.value("hypotheses").toInt()>4000);
        QVERIFY(object.value("best").toObject().value("ncc").toDouble()>.99);
        const QString screenshot=qEnvironmentVariable("FPSTUDIO_LIVE_SCREENSHOT");
        if(!screenshot.isEmpty())QVERIFY(w.grab().save(screenshot));
        QTest::mouseClick(w.findChildren<QPushButton *>().first(),Qt::LeftButton);
        QTRY_COMPARE_WITH_TIMEOUT(w.findChildren<QPushButton *>().first()->text(),QStringLiteral("닫기"),2000);
    }
    void selectorRejectsBadFrames() {
        QImage flat(108,88,QImage::Format_Grayscale8); flat.fill(128);
        QVERIFY(!fpstudio::assessPreview(flat, flat, 500, 78, 30).eligible);
        QImage ridges = flat;
        for (int y=0;y<88;++y) for(int x=0;x<108;++x) ridges.scanLine(y)[x] = (x/3)%2 ? 220 : 30;
        QVERIFY(fpstudio::assessPreview(ridges, ridges, 500, 78, 30).eligible);
        QVERIFY(!fpstudio::assessPreview(ridges, ridges, 0, 78, 30).eligible);
        QVERIFY(!fpstudio::assessPreview(ridges, ridges, 500, 30, 30).eligible);
        QVERIFY(!fpstudio::assessPreview(ridges, ridges, 500, 78, 5).eligible);
        QVERIFY(!fpstudio::assessPreview(ridges, flat, 500, 78, 30).eligible);
    }
    void livePreviewAndStop() {
        QTemporaryDir dir;
        fpstudio::LiveWindow w(dir.path(), nullptr, FAKE_CAPTURE, 1);
        w.show();
        QCOMPARE(w.findChildren<QPushButton *>().size(), 1);
        auto liveStatus = [&] {
            QFile f(dir.path() + "/live-status.json");
            if (!f.open(QIODevice::ReadOnly)) return QJsonObject();
            return QJsonDocument::fromJson(f.readAll()).object();
        };
        QTRY_VERIFY_WITH_TIMEOUT(liveStatus().value("frames").toInt() >= 20, 3000);
        QTRY_VERIFY_WITH_TIMEOUT(liveStatus().value("fps").toDouble() >= 15, 3000);
        QCOMPARE(liveStatus().value("candidates").toInt(), 1);
        QVERIFY(QFile::exists(dir.path()+"/candidate-touch-01.png"));
        QTRY_COMPARE_WITH_TIMEOUT(liveStatus().value("fps").toDouble(), 0.0, 4000);
        QVERIFY(liveStatus().value("instruction").toString().contains(QStringLiteral("멈췄습니다")));
        QTest::mouseClick(w.findChildren<QPushButton *>().first(), Qt::LeftButton);
        QTRY_COMPARE_WITH_TIMEOUT(w.findChildren<QPushButton *>().first()->text(), QStringLiteral("닫기"), 2000);
    }
    void fullPlan() {
        QTemporaryDir dir;
        fpstudio::ResearchWindow w(dir.path(), nullptr, FAKE_CAPTURE, 1, 1);
        w.show();
        const QString screenshot = qEnvironmentVariable("FPSTUDIO_TEST_SCREENSHOT");
        if (!screenshot.isEmpty()) w.grab().save(screenshot);
        QCOMPARE(status(dir.path()).value("state").toString(), "ready");
        QCOMPARE(w.findChildren<QPushButton *>().size(), 1);
        QTRY_COMPARE_WITH_TIMEOUT(status(dir.path()).value("state").toString(), "await-touch", 1500);
        QTRY_COMPARE_WITH_TIMEOUT(status(dir.path()).value("state").toString(), "complete", 15000);
        const auto samples = status(dir.path()).value("samples").toArray();
        QCOMPARE(samples.size(), 14);
        QCOMPARE(samples.at(5).toObject().value("role").toString(), "reference");
        QCOMPARE(samples.at(6).toObject().value("role").toString(), "probe");
        QCOMPARE(samples.at(10).toObject().value("finger").toString(), "middle");
        for (const auto &sample : samples) QVERIFY(QFile::exists(sample.toObject().value("path").toString()));
        w.grab().save(dir.path() + "/complete.png");
    }
    void retriesPauseAndCancel() {
        qputenv("FPSTUDIO_TEST_FAIL", "1");
        QTemporaryDir dir;
        fpstudio::ResearchWindow w(dir.path(), nullptr, FAKE_CAPTURE, 1, 1);
        QTRY_COMPARE_WITH_TIMEOUT(status(dir.path()).value("state").toString(), "paused", 5000);
        QCOMPARE(status(dir.path()).value("attempts").toArray().size(), 3);
        QCOMPARE(status(dir.path()).value("completed").toInt(), 0);
        QFile stop(dir.path() + "/stop"); QVERIFY(stop.open(QIODevice::WriteOnly)); stop.close();
        QTRY_COMPARE(status(dir.path()).value("state").toString(), "cancelled");
        qunsetenv("FPSTUDIO_TEST_FAIL");
    }
    void duplicateSession() {
        QTemporaryDir a, b;
        fpstudio::ResearchWindow first(a.path(), nullptr, FAKE_CAPTURE, 1, 1);
        fpstudio::ResearchWindow second(b.path(), nullptr, FAKE_CAPTURE, 1, 1);
        QVERIFY(QFile::exists(a.path() + "/session.json"));
        QVERIFY(!QFile::exists(b.path() + "/session.json"));
    }
    void duplicateProcesses() {
        QTemporaryDir dir;
        QProcess first, second;
        auto environment=QProcessEnvironment::systemEnvironment();
        environment.insert("XDG_RUNTIME_DIR",dir.path());
        first.setProcessEnvironment(environment);second.setProcessEnvironment(environment);
        first.start(FPSTUDIO_APP, {"--research", "--preview", "--session-dir", dir.path()});
        QVERIFY(first.waitForStarted());
        QTRY_VERIFY(QFile::exists(dir.path() + "/session.json"));
        second.start(FPSTUDIO_APP, {"--research", "--preview", "--session-dir", dir.path() + "/duplicate"});
        QVERIFY(second.waitForFinished(3000));
        QCOMPARE(second.exitCode(), 3);
        first.terminate(); QVERIFY(first.waitForFinished(3000));

        first.start(FPSTUDIO_APP, {"--mcp"}); QVERIFY(first.waitForStarted());
        first.write("{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\"}\n");
        QVERIFY(first.waitForReadyRead(3000));
        second.start(FPSTUDIO_APP, {"--mcp"}); QVERIFY(second.waitForFinished(3000));
        QCOMPARE(second.exitCode(), 3);
        first.closeWriteChannel(); QVERIFY(first.waitForFinished(3000));
    }
};
QTEST_MAIN(ResearchTest)
#include "research_test.moc"
