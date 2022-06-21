#include "patcher.h"
#include <QDataStream>
#include <QCryptographicHash>

Patcher::Patcher(QString patchFileName)
{
    QFile file(patchFileName);
    if(!file.open(QIODevice::ReadOnly)){
        Q_ASSERT(0);
    }

    QDataStream in(&file);
    originalSumm.reserve(16);
    originalSumm.resize(in.readRawData(originalSumm.data(), 16));
    patchedSumm.reserve(16);
    patchedSumm.resize( in.readRawData(patchedSumm.data(), 16));
    while(!in.atEnd()){
        patchlist.resize(patchlist.size() + 1);
        in >> patchlist.back();
    }

    file.close();
}

void Patcher::patch(QFile &file)
{
    if(checkFile(file) != State::Original) return;

    file.open(QIODevice::ReadWrite);
    for(int i = 0; i < patchlist.size(); ++i){
        file.seek(patchlist[i].address);
        file.write(patchlist[i].patch.data(), patchlist[i].length);
    }
    file.close();
}

Patcher::State Patcher::checkFile(QFile &file)
{
    if(file.open(QIODevice::ReadOnly)){
        QCryptographicHash hash(QCryptographicHash::Algorithm::Md5);
        hash.addData(&file);
        file.close();
        QByteArray result = hash.result();
        if(result == originalSumm){
            return State::Original;
        }
        if(result == patchedSumm){
            return State::Patched;
        }
    }
    return State::Unknown;
}

QDataStream &operator>>(QDataStream& in, PatchNote& note)
{
    in.readRawData((char*)&note.address, sizeof(int));
    in.readRawData((char*)&note.length, sizeof(int));
    note.patch.reserve(note.length);
    note.patch.resize(in.readRawData(note.patch.data(), note.length));
    return in;
}


