#include "authpolicy.h"
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QJsonArray>
#include <QJsonDocument>
#include <QProcess>
#include <QProcessEnvironment>
#include <QRegularExpression>
#include <QTemporaryDir>
#include <cstdio>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <signal.h>
#include <unistd.h>

static bool trusted(const QString &path,bool directory=false) {
    QString cursor=path;
    bool leaf=true;
    while(cursor!="/") {
        struct stat st{};
        if(lstat(QFile::encodeName(cursor).constData(),&st)||st.st_uid!=0||
           (st.st_mode&0022)||S_ISLNK(st.st_mode)||
           (leaf&&!directory?!S_ISREG(st.st_mode):!S_ISDIR(st.st_mode)))return false;
        leaf=false;cursor=QFileInfo(cursor).absolutePath();
    }
    return true;
}
int main(int argc,char **argv) {
    QCoreApplication app(argc,argv);
    // This is an ordinary executable, NEVER setuid. fprintd is the caller and
    // retains its own D-Bus authorization / enrolled-user selection.
    if(getuid()!=0||geteuid()!=0||argc!=3)return 2;
    const QString mode=QString::fromLocal8Bit(argv[1]);
    if(mode!="--user"&&mode!="--self-test")return 2;
    const QString user=QString::fromLocal8Bit(argv[2]);
    if(!QRegularExpression("^[a-z_][a-z0-9_-]{0,31}$").match(user).hasMatch())return 2;
    const auto pw=getpwnam(argv[2]);if(!pw||pw->pw_uid==0)return 2;
    const QString root="/opt/fpstudio-auth";
    const QString bank="/var/lib/fprint/fpstudio-gpu/"+user;
    const QString manifest=bank+"/manifest.json";
    if(!trusted("/etc/fpstudio-auth-users/"+user)||!trusted(manifest)||!trusted(root+"/bin/fpstudio-vkmatch")||
       !trusted(root+"/bin/fpstudio-match.spv"))return 2;
    QFile file(manifest);if(!file.open(QIODevice::ReadOnly)||file.size()>1024*1024)return 2;
    QJsonParseError parse;
    const auto config=QJsonDocument::fromJson(file.readAll(),&parse).object();
    if(parse.error!=QJsonParseError::NoError||config.value("schema_version").toInt()!=1||
       config.value("username").toString()!=user||config.value("uid").toInt(-1)!=int(pw->pw_uid)||
       config.value("policy").toString()!="experimental-gpu-v1"||
       !config.value("experimental_auth_enabled").toBool())return 2;
    QStringList args{"--shader",root+"/bin/fpstudio-match.spv","--auth-search"};
    const auto references=config.value("references").toArray();
    if(references.isEmpty()||references.size()>64)return 2;
    QSet<QString> hashes;QString firstReference;
    for(const auto &item:references) {
        const auto entry=item.toObject();const QString name=entry.value("file").toString();
        if(!QRegularExpression("^reference-index-[0-9]{3}\\.png$").match(name).hasMatch())return 2;
        QFile ref(bank+"/"+name);if(!trusted(ref.fileName())||!ref.open(QIODevice::ReadOnly)||ref.size()>1024*1024)return 2;
        const auto bytes=ref.readAll();
        const QString hash=QString::fromLatin1(QCryptographicHash::hash(bytes,QCryptographicHash::Sha256).toHex());
        if(hash!=entry.value("sha256").toString()||hashes.contains(hash)||QImage::fromData(bytes).size()!=QSize(108,88))return 2;
        hashes.insert(hash);args<<"--reference"<<ref.fileName();
        if(firstReference.isEmpty())firstReference=ref.fileName();
    }
    // Fixed-size PGM transport, no paths or commands accepted from the caller.
    QByteArray bytes;
    if(mode=="--self-test") {
        // Transport/install smoke test only; never claimed as recognition validation.
        const auto im=QImage(firstReference).convertToFormat(QImage::Format_Grayscale8);
        bytes="P5\n108 88\n255\n";
        for(int y=0;y<88;++y)bytes.append(reinterpret_cast<const char *>(im.constScanLine(y)),108);
    } else {
        QFile input;if(!input.open(stdin,QIODevice::ReadOnly))return 2;
        bytes=input.read(10000);
    }
    if(bytes.size()!=9518||!bytes.startsWith("P5\n108 88\n255\n"))return 2;
    QTemporaryDir work("/tmp/fpstudio-auth-XXXXXX");if(!work.isValid())return 2;
    const auto probe=work.path()+"/probe.pgm";
    QFile image(probe);if(!image.open(QIODevice::WriteOnly))return 2;
    image.setPermissions(QFileDevice::ReadOwner|QFileDevice::WriteOwner);
    if(image.write(bytes)!=bytes.size())return 2;
    image.close();
    args<<"--probe"<<probe;
    QProcess worker;QProcessEnvironment env;
    env.insert("PATH","/usr/bin");env.insert("LANG","C.UTF-8");
    env.insert("VK_DRIVER_FILES","/usr/share/vulkan/icd.d/radeon_icd.json");
    worker.setProcessEnvironment(env);
    const pid_t parent=getpid();
    worker.setChildProcessModifier([parent]{
        if(prctl(PR_SET_PDEATHSIG,SIGKILL)||getppid()!=parent)_exit(127);
    });
    worker.start(root+"/bin/fpstudio-vkmatch",args);
    if(!worker.waitForFinished(4000)){worker.kill();worker.waitForFinished(1000);return 2;}
    const auto output=worker.readAllStandardOutput();
    if(worker.exitStatus()!=QProcess::NormalExit||worker.exitCode()!=0||output.size()>2*1024*1024)return 2;
    const auto result=QJsonDocument::fromJson(output,&parse).object();
    if(parse.error!=QJsonParseError::NoError||!result.value("ok").toBool()||
       result.value("algorithm_version").toInt()!=8||
       result.value("auth_profile").toString()!=QStringLiteral("auth-v8-contact-anchored-ridge-roi-uniform-scale-5pct")||
       !result.value("auth_search").toBool())return 2;
    const auto best=result.value("best").toObject(),e=best.value("interior").toObject();
    const auto roi=best.value("ridge_roi").toObject();
    if(!roi.value("binary").toBool()||roi.value("version").toInt()!=1||
       !std::isfinite(roi.value("overlap").toDouble())||roi.value("overlap").toDouble()<.50||
       roi.value("reference_mask").toInt()==0||roi.value("probe_mask").toInt()==0)return 2;
    // Numeric diagnostics only: never log biometric images, paths or user IDs.
    std::fprintf(stderr,"fpstudio GPU v8 accepted=%d ncc=%.3f gradient=%.3f overlap=%.3f roi=%.3f tiles=%d/%d ambiguous=%d boundary=%d refined=%d ms=%.1f\n",
        authPatternAccepted(result),e.value("ncc").toDouble(),e.value("gradient").toDouble(),
        e.value("overlap").toDouble(),roi.value("overlap").toDouble(),e.value("supported_tiles").toInt(),e.value("probe_tiles").toInt(),
        best.value("ambiguous").toBool(),best.value("at_search_boundary").toBool(),
        result.value("precision_hypotheses").toInteger()>0,result.value("wall_ms").toDouble());
    if(!authPatternAccepted(result)){std::fputs("FPSTUDIO_GPU_NO_MATCH_V1\n",stdout);return 1;}
    std::fputs("FPSTUDIO_GPU_MATCH_V1\n",stdout);return 0;
}
