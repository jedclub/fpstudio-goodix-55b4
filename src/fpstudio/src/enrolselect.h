#pragma once
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include <algorithm>
#include <cmath>

// Choosing which of a session's captures become the enrolment.
//
// This lives apart from the window that produced them for two reasons. It was
// unreachable except by pressing a button, so it could not be tested and was
// shipped unexercised; and it read the window's own memory, so once a session
// ended the choice it would have made could not be reproduced or checked. Both
// are fixed by the same move: the poses are written beside the captures, and
// the selection is a function of a directory.
namespace fpstudio {

struct EnrolCandidate {
    QString path;
    int x = 0, y = 0;        // where the capture sits on the coverage map
    double quality = 0;      // ridge sharpness, as the capture path measures it
};

// Every capture in a session directory that recorded where it landed.
//
// A capture that never registered has no place on the finger and is skipped:
// including it would mean enrolling an image without knowing what it shows.
inline QVector<EnrolCandidate> readEnrolCandidates(const QString &directory)
{
    QVector<EnrolCandidate> pool;
    QDir dir(directory);
    const auto names = dir.entryList({QStringLiteral("candidate-touch-*.png"),
                                      QStringLiteral("candidate-sweep-*.png")},
                                     QDir::Files, QDir::Name);
    for(const QString &name:names) {
        const QString path=dir.absoluteFilePath(name);
        QFile sidecar(path+QStringLiteral(".json"));
        if(!sidecar.open(QIODevice::ReadOnly))continue;
        const auto row=QJsonDocument::fromJson(sidecar.readAll()).object();
        if(!row.contains(QStringLiteral("map_x")))continue;
        EnrolCandidate c;
        c.path=path;
        c.x=row.value(QStringLiteral("map_x")).toInt();
        c.y=row.value(QStringLiteral("map_y")).toInt();
        c.quality=row.value(QStringLiteral("map_quality")).toDouble();
        pool.push_back(c);
    }
    return pool;
}

// Grow the set by what each capture adds rather than by how sharp it is.
//
// Ranking on sharpness alone was measured and is the wrong rule: on one
// session the sharpest 21 captures recognised 17 of 25 probes where a broader
// 32 recognised 20. A set chosen for sharpness clusters on whatever part of
// the finger happened to press cleanly and has nothing to say about the rest,
// and the parts it has nothing to say about are exactly where authentication
// fails - a lock screen is approached at a different angle from a terminal.
//
// Cells rather than pixels: the question is whether a capture reaches
// somewhere new, and a few pixels is not somewhere new. Sharpness breaks ties
// and fills any room left over, because more views of a place already covered
// still help, just less.
inline QStringList selectEnrolment(const QVector<EnrolCandidate> &pool, int limit,
                                   int mapSize, int captureWidth = 108,
                                   int captureHeight = 88, int cell = 12)
{
    QStringList chosen;
    if(pool.isEmpty()||limit<=0||mapSize<=0||cell<=0)return chosen;
    const int grid=(mapSize+cell-1)/cell;
    QVector<bool> taken(grid*grid,false);
    QVector<bool> used(pool.size(),false);

    auto cellsOf=[&](const EnrolCandidate &c){
        QVector<int> cells;
        for(int y=c.y;y<c.y+captureHeight;y+=cell)
            for(int x=c.x;x<c.x+captureWidth;x+=cell) {
                const int cx=x/cell,cy=y/cell;
                if(cx>=0&&cy>=0&&cx<grid&&cy<grid)cells.push_back(cy*grid+cx);
            }
        return cells;
    };

    while(chosen.size()<limit) {
        int best=-1;double bestGain=0;
        for(int i=0;i<pool.size();++i) {
            if(used[i])continue;
            int fresh=0;
            for(int c:cellsOf(pool[i]))if(!taken[c])++fresh;
            const double gain=fresh*std::max(0.05,pool[i].quality);
            if(gain>bestGain){bestGain=gain;best=i;}
        }
        if(best<0)break;                       // nothing reaches anywhere new
        used[best]=true;
        for(int c:cellsOf(pool[best]))taken[c]=true;
        chosen<<pool[best].path;
    }
    if(chosen.size()<limit) {
        QVector<int> rest;
        for(int i=0;i<pool.size();++i)if(!used[i])rest.push_back(i);
        std::sort(rest.begin(),rest.end(),
                  [&](int a,int b){return pool[a].quality>pool[b].quality;});
        for(int i:rest){ if(chosen.size()>=limit)break; chosen<<pool[i].path; }
    }
    return chosen;
}

}
