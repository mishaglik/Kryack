#ifndef PATCHER_H
#define PATCHER_H
#include <QFile>
#include <QByteArray>
#include <QVector>
#include <QDataStream>

struct PatchNote{
    int address;
    int length;
    QByteArray patch;
};

QDataStream &operator>>(QDataStream& in, PatchNote& note);

class Patcher
{
public:
    enum class State{
        Original,
        Patched,
        Unknown,
    };
    Patcher(QString patchFileName);
    void patch(QFile &file);
    State checkFile(QFile &file);
private:
    QVector<PatchNote> patchlist;
    QByteArray originalSumm;
    QByteArray patchedSumm;
};

#endif // PATCHER_H
