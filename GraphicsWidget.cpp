#include "GraphicsWidget.h"
#include <QDebug>
#include <QPaintEvent>
#include <QLabel>
#include <QThread>
#include <QTimer>
#include <QEventLoop>
#include <time.h>


GraphicsWidget::GraphicsWidget(QWidget *parent) :
    QFrame(parent)
{
    grid = new QGridLayout(this);
    setLayout(grid);
    setStyleSheet("QFrame{border-radius: 10px; background-color: #e9f3fb;}");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    grid->setSpacing(10);
    PixmapLabel::loadBackSide(":/images/back.png");
    for (int i = 0; i < 10; i++)
    {
        pixmaps[i].load(":/images/" + QString::number(i+1) + ".png");
    }
    PixmapLabel* label;
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 5; col++)
        {
            label  = new PixmapLabel();
            int pixId = (row%2) ? col+5 :col;
            label->setPixmap(pixmaps[pixId], pixId);
            grid->addWidget(label, row, col);
            connect(label, &PixmapLabel::cardClicked, this, &GraphicsWidget::onCardClicked);
        }
    }

    loop = new QEventLoop();
    timer = new QTimer();
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, loop, &QEventLoop::quit);

    sndThread = new QThread;
    effects = new SoundEffects;
    effects->moveToThread(sndThread);
    connect(sndThread, &QThread::finished, effects, &SoundEffects::deleteLater);
    connect(this, &GraphicsWidget::play, effects, &SoundEffects::play);
    connect(this, &GraphicsWidget::destroyed, sndThread, &QThread::deleteLater);

    sndThread->start();

    lastSelection = 0;
    countFound = 0;
    score = 0;
    effectsOn = true;
    firstclick = true;
}

GraphicsWidget::~GraphicsWidget()
{
    delete grid;
    delete loop;
    delete timer;
    delete effects;
}

void GraphicsWidget::onCardClicked(PixmapLabel *actSelection)
{
    if (firstclick)
    {
        firstclick = false;
        emit firstclicked();
    }
    actSelection->flip();
    if (lastSelection)
    {
        if (actSelection->getId() == lastSelection->getId())
        {
            lastSelection = 0;
            score += 20-2*countFound;
            countFound++;
            emit result(score);
            if (countFound == 10)
            {
                if (effectsOn) emit play(SoundEffects::FINISHED);
                emit waitForPlayerDlg();
                timer->start(1000); // kicsit várunk
                loop->exec(QEventLoop::ExcludeUserInputEvents);
                emit finished();
            }
            else
            {
                if (effectsOn) emit play(SoundEffects::FOUND);
            }
        }
        else
        {
            score -= 2;
            if (effectsOn) emit play(SoundEffects::NOTFOUND);
            emit result(score);
            timer->start(1000);
            loop->exec(QEventLoop::ExcludeUserInputEvents);
            lastSelection->flip();
            actSelection->flip();
            lastSelection = 0;
        }
    }
    else
    {
        if (effectsOn) emit play(SoundEffects::CLICKED);
        lastSelection = actSelection;
    }
}

void GraphicsWidget::dealCard(CardDealer* dealer)
{
    // kártyalesöprés animálása
    if (effectsOn) emit play(SoundEffects::START);
    dealer->setDuration(2000);
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 5; col++)
        {
            PixmapLabel* label = (PixmapLabel*)grid->itemAtPosition(row, col)->widget();
            dealer->append(*label);
            label->fill();
        }
    }
    emit clearTable();
    loop->exec();
    // kártyosztás animálása
    if (effectsOn) emit play(SoundEffects::NEWDEAL);
    dealer->setDuration(4000);
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 5; col++)
        {
            PixmapLabel* label = (PixmapLabel*)grid->itemAtPosition(row, col)->widget();
            dealer->append(*label, false);
        }
    }
    emit clearTable();
    loop->exec();

    srand(time(NULL));
    QList<int> rlist;   // random 0-19, feltöltjük
    QList<int> foundlist;  // check, ha már volt, kivesszük
    for (int i = 0; i < 20; i++)
        foundlist.append(i);
    while (!foundlist.isEmpty())
    {
        int v = rand()%20;
        int ix = foundlist.indexOf(v);
        if (ix > -1)
        {
            foundlist.removeAt(ix);
            rlist.append(v);
        }
    }

    for (int id = 0; id < 10; id++)
    {
        for (int j = 0; j < 2; j++)
        {
            int idx = rlist.at(0);
            rlist.removeAt(0);
            int row = (int)floor(idx/5);
            int col = idx-5*row;
            PixmapLabel* label = (PixmapLabel*)grid->itemAtPosition(row, col)->widget();
            label->setPixmap(pixmaps[id], id);
        }
    }

    score = 0;
    lastSelection = 0;
    countFound = 0;
    firstclick = true;
}

void GraphicsWidget::dealReady()
{
    loop->exit();
}

