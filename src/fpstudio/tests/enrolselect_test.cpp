// What a session's captures should become, and what they should not.
//
// This selection used to live inside the enrolment window and read its memory,
// so it ran only when somebody pressed a button and could not be checked at
// all. It shipped unexercised twice. It is a function of a directory now, and
// this is the exercise.
#include "enrolselect.h"
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>
#include <QTemporaryDir>
#include <QtTest>

using fpstudio::EnrolCandidate;
using fpstudio::readEnrolCandidates;
using fpstudio::selectEnrolment;

namespace {
void write(const QDir &dir, const QString &name, QJsonObject row)
{
    QSaveFile image(dir.absoluteFilePath(name));
    QVERIFY(image.open(QIODevice::WriteOnly));
    image.write("\x89PNG\r\n\x1a\n");
    QVERIFY(image.commit());
    QSaveFile note(dir.absoluteFilePath(name + ".json"));
    QVERIFY(note.open(QIODevice::WriteOnly));
    note.write(QJsonDocument(row).toJson());
    QVERIFY(note.commit());
}
}

class EnrolSelectTest : public QObject {
    Q_OBJECT
private slots:
    void coversTheFingerRatherThanTheSharpestCorner() {
        // Ten sharp views of one place, and three ordinary ones elsewhere. The
        // sharp cluster is what ranking on quality alone would take, and it is
        // the wrong answer: it enrols one corner of the finger and leaves the
        // rest unrecognised.
        QVector<EnrolCandidate> pool;
        for(int i=0;i<10;++i)pool.push_back({QStringLiteral("sharp-%1").arg(i),20,20,0.90});
        pool.push_back({QStringLiteral("far-a"),150,20,0.40});
        pool.push_back({QStringLiteral("far-b"),20,150,0.40});
        pool.push_back({QStringLiteral("far-c"),150,150,0.40});
        const auto chosen=selectEnrolment(pool,4,300);
        QCOMPARE(chosen.size(),4);
        for(const char *name:{"far-a","far-b","far-c"})
            QVERIFY2(chosen.contains(QString::fromLatin1(name)),
                     qPrintable(chosen.join(',')));
    }

    void roomLeftOverGoesToTheClearestViews() {
        QVector<EnrolCandidate> pool{{QStringLiteral("dull"),20,20,0.20},
                                     {QStringLiteral("keen"),20,20,0.80},
                                     {QStringLiteral("other"),150,150,0.50}};
        const auto chosen=selectEnrolment(pool,3,300);
        QCOMPARE(chosen.size(),3);
        QVERIFY(chosen.indexOf(QStringLiteral("keen"))<chosen.indexOf(QStringLiteral("dull")));
    }

    void neverExceedsTheLimitAndSurvivesNothing() {
        QVector<EnrolCandidate> pool;
        for(int i=0;i<40;++i)pool.push_back({QStringLiteral("c-%1").arg(i),i*4,i*3,0.5});
        QCOMPARE(selectEnrolment(pool,7,300).size(),7);
        QVERIFY(selectEnrolment({},10,300).isEmpty());
        QVERIFY(selectEnrolment(pool,0,300).isEmpty());
    }

    void readsBothKindsOfCaptureAndSkipsUnplacedOnes() {
        QTemporaryDir session;
        QVERIFY(session.isValid());
        const QDir dir(session.path());
        write(dir,QStringLiteral("candidate-touch-00.png"),
              {{"map_x",10},{"map_y",20},{"map_quality",0.5}});
        write(dir,QStringLiteral("candidate-sweep-000.png"),
              {{"map_x",90},{"map_y",90},{"map_quality",0.6}});
        // Registered nowhere: enrolling it would mean enrolling an image
        // without knowing what part of the finger it shows.
        write(dir,QStringLiteral("candidate-touch-01.png"),
              {{"role","unvalidated-candidate"}});
        const auto pool=readEnrolCandidates(session.path());
        QCOMPARE(pool.size(),2);
        const auto chosen=selectEnrolment(pool,8,300);
        QCOMPARE(chosen.size(),2);
        for(const QString &p:chosen)QVERIFY(!p.contains(QStringLiteral("touch-01")));
    }

    void anEmptyOrUnreadableSessionSelectsNothing() {
        QTemporaryDir empty;
        QVERIFY(empty.isValid());
        QVERIFY(readEnrolCandidates(empty.path()).isEmpty());
        QVERIFY(readEnrolCandidates(QStringLiteral("/nonexistent-session")).isEmpty());
    }
};

QTEST_MAIN(EnrolSelectTest)
#include "enrolselect_test.moc"
