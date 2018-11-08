#include "SoundEffects.h"
#include <QDebug>

SoundEffects::SoundEffects()
{
    qRegisterMetaType<SoundId>("SoundId");
    url[CLICKED] = QUrl::fromLocalFile(":/sounds/clicked.wav");
    url[FOUND] = QUrl::fromLocalFile(":/sounds/found.wav");
    url[NOTFOUND] = QUrl::fromLocalFile(":/sounds/notfound.wav");
    url[FINISHED] = QUrl::fromLocalFile(":/sounds/finished.wav");
    url[START] = QUrl::fromLocalFile(":/sounds/start.wav");
    url[NEWDEAL] = QUrl::fromLocalFile(":/sounds/newDeal.wav");
    for (int i = 0; i < 6; i++)
    {
        effect[i] = new QSoundEffect;
        effect[i]->setSource(url[i]);
        effect[i]->setVolume(1);
        effect[i]->setLoopCount(1);
    }
}

SoundEffects::~SoundEffects()
{
    for (int i = 0; i < 6; i++)
        delete effect[i];
}

void SoundEffects::play(SoundId id)
{
    if (effect[id]->isPlaying()){
        effect[id]->stop();
        effect[id]->play();
    }
    else
    {
        effect[id]->play();
    }
}
