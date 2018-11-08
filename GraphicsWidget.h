#ifndef GRAPHICSWIDGET_H
#define GRAPHICSWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QGridLayout>
#include <QFrame>
#include "PixmapLabel.h"
#include "SoundEffects.h"
#include "CardDealer.h"

class GraphicsWidget : public QFrame
{
    Q_OBJECT

public:
    explicit GraphicsWidget(QWidget *parent = 0);
    ~GraphicsWidget();
public slots:
    void onCardClicked(PixmapLabel* actSelection);
    void dealCard(CardDealer* dealer);
    void setEffects(bool on){effectsOn = on;}
    void dealReady();
signals:
    void finished();
    void result(int val);
    void waitForPlayerDlg();
    void play(SoundEffects::SoundId id);
    void clearTable();
    void firstclicked();
private:
    QPixmap pixmaps[10];
    QGridLayout* grid;
    PixmapLabel* lastSelection;
    int countFound;
    int score;
    QEventLoop* loop;
    QTimer* timer;
    SoundEffects* effects;
    QThread* sndThread;
    bool effectsOn;
    bool firstclick;
};

#endif // GRAPHICSWIDGET_H
