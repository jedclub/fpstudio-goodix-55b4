// A synthetic device process for state-machine tests. Never opens USB.
#include <QCoreApplication>
#include <QFile>
#include <QImage>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>
#include <QTimer>
#include <cstdio>
#include <QThread>
#include <iostream>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    const auto args = app.arguments();
    if(args.contains("--serve")) {
        QThread::msleep(2000); // Exercise candidates arriving before readiness.
        std::cout << "{\"ready\":true,\"device\":\"synthetic-test-only\",\"references\":1}" << std::endl;
        std::string line;
        while(std::getline(std::cin,line)) {
            QThread::msleep(450); // Slower than contacts: exercise queued IDs/drain.
            const auto request=QJsonDocument::fromJson(QByteArray::fromStdString(line)).object();
            const QJsonObject result{{"ok",true},{"synthetic_test_only",true},
                {"probe",request.value("probe")},{"authentication_decision",QJsonValue::Null},
                {"best",QJsonObject{{"interior",QJsonObject{{"consistent",!qEnvironmentVariableIsSet("FPSTUDIO_TEST_RETRY")}}}}}};
            std::cout << QJsonDocument(result).toJson(QJsonDocument::Compact).constData() << std::endl;
        }
        return 0;
    }
    const QString path = args.value(args.indexOf("--out") + 1);
    const QString preview = qEnvironmentVariable("GOODIX55X4_PREVIEW_PATH");
    QTimer stream;
    qint64 stamp = 0;
    const int contacts=qEnvironmentVariableIntValue("FPSTUDIO_TEST_CONTACTS");
    const bool retry=qEnvironmentVariableIsSet("FPSTUDIO_TEST_RETRY");
    const bool extended=qEnvironmentVariableIsSet("FPSTUDIO_TEST_EXTENDED");
    if (!preview.isEmpty()) {
        QObject::connect(&stream, &QTimer::timeout, [&] {
            if (QFile::exists(preview + ".stop")) { app.exit(); return; }
            // Pause after 30 frames to exercise stale-image detection.
            if(contacts&&stamp>=contacts*10){app.exit();return;}
            if(retry&&stamp>=120){app.exit();return;}
            if (!contacts&&!retry&&stamp >= 30) return;
            ++stamp;
            const bool empty=contacts&&(stamp-1)%10<3;
            const bool bad=contacts&&stamp<=10;
            QByteArray pixels(108 * 88, 0);
            for (int y = 0; y < 88; ++y)
                for (int x = 0; x < 108; ++x) pixels[y*108+x] = ((x+(retry?int(stamp/20)%3:0)) / 3) % 2 ? 220 : 30;
            QSaveFile frame(preview);
            if (frame.open(QIODevice::WriteOnly)) {
                const QByteArray extra=extended?(QByteArray(empty?" 220":" 800")+(stamp<=3?" 0":" 1")+
                    (empty||stamp<=3?" 0":" 1")+" 4.0"):QByteArray();
                frame.write("P5\n# " + QByteArray::number(stamp) + (empty?" 0": " 500") +
                    (bad?" 30":" 78") + " 30"+extra+"\n108 88\n255\n" + pixels);
                frame.commit();
            }
        });
        stream.start(40);
        return app.exec();
    }
    QTimer::singleShot(50, [&] {
        QSaveFile beacon(qEnvironmentVariable("FPSTUDIO_STATUS_PATH"));
        if (beacon.open(QIODevice::WriteOnly)) {
            beacon.write(QJsonDocument(QJsonObject{{"pid", app.applicationPid()}, {"stage", "background-ready"}}).toJson());
            beacon.commit();
        }
    });
    QTimer::singleShot(350, [&] {
        const bool fail = qEnvironmentVariableIsSet("FPSTUDIO_TEST_FAIL");
        if (!fail) { QImage image(108, 88, QImage::Format_Grayscale8); image.fill(128); image.save(path); }
        const QByteArray result = QJsonDocument(QJsonObject{{"ok", !fail}, {"error", fail ? "synthetic retry" : ""}}).toJson(QJsonDocument::Compact);
        std::puts(result.constData());
        app.exit(fail ? 1 : 0);
    });
    return app.exec();
}
