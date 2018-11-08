#ifndef SOUNDEFFECTS_H
#define SOUNDEFFECTS_H
#include <QObject>
#include <QSoundEffect>
#include <QSound>

class SoundEffects : public QObject
{
    Q_OBJECT
public:
    SoundEffects();
    ~SoundEffects();
    enum SoundId
    {
        CLICKED,
        START,
        FOUND,
        NOTFOUND,
        FINISHED,
        NEWDEAL,
    };
    Q_ENUM(SoundId)


public slots:
    void play(SoundId id);
private:
    QUrl url[6];
    QSoundEffect* effect[6];
};

#endif // SOUNDEFFECTS_H
