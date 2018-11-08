#ifndef CARDDEALER_H
#define CARDDEALER_H

#include <QObject>
#include <QFrame>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QTransform>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QList>
#include <QTimeLine>
#include <QPixmap>
#include "PixmapLabel.h"

class CardDealer;
class MovedCard : public QLabel
{
    Q_OBJECT
    friend class CardDealer;
public:
    MovedCard(QWidget* parent, bool direction = true);
    virtual ~MovedCard();
    void setRoute(QPoint p1, QPoint p2);
    void setRotation(qreal ang1, qreal ang2);
    void setSize(qreal sx, qreal sy);
private:
    QPropertyAnimation animrot;
    QPropertyAnimation animpos;
    QPropertyAnimation animsize;
    QParallelAnimationGroup group;
    bool rotOn;
    bool posOn;
    bool sizeOn;
};

typedef QList<MovedCard*> CardList;
typedef QList<MovedCard*>::iterator itCardList;


class CardDealer : public QWidget
{
    Q_OBJECT
public:
    explicit CardDealer(QWidget* parent);
    ~CardDealer();
    void append(const PixmapLabel& label, bool clearTable = true);
    void setDuration(int msec);
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
private:
    QTimeLine* timeline;
    CardList cardlist;
    int duration;
    QPoint bottomRightCorner;
    QPoint bottomLeftCorner;
//    qreal timeval;
signals:
    void startMove();
    void finished();
private slots:
    void endMove();
    void timeChanged();
public slots:
    void start();
};

#endif // CARDDEALER_H
