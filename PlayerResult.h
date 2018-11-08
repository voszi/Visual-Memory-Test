#ifndef PLAYERRESULT_H
#define PLAYERRESULT_H
#include <QString>
#include <QList>

typedef struct t_PlayerResult
{
    QString player;
    QString bestResult;
    QString lastResult;
    QString bestTime;
    QString lastTime;
} PlayerResult;

typedef QList<PlayerResult> PlayerResultList;
typedef QList<PlayerResult>::iterator playPlayerResultIterator;

#endif // PLAYERRESULT_H
