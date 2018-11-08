#include "CardDealer.h"
#include <QPainter>
#include <QDebug>
#include <QResizeEvent>


MovedCard::MovedCard(QWidget* parent, bool direction) : QLabel(parent)
{
    animpos.setTargetObject(this);
    animpos.setDirection(direction ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
    animpos.setStartValue(QPointF(0, 0));
    animpos.setEndValue(QPointF(1000, 600));
    animpos.setDuration(1000); // default
    posOn = false;

    animsize.setTargetObject(this);
    animsize.setStartValue(1.0);
    animsize.setEndValue(0.1);
    animsize.setDuration(1000);
    animsize.setDirection(direction ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
    sizeOn = false;

    animrot.setTargetObject(this);
    animrot.setStartValue(0);
    animrot.setEndValue(0);
    animrot.setDuration(1000);
    animrot.setDirection(direction ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
    rotOn = false;

    group.addAnimation(&animpos);
    group.addAnimation(&animsize);
    group.addAnimation(&animrot);

    hide();

}

MovedCard::~MovedCard()
{
    group.removeAnimation(&animpos);
    group.removeAnimation(&animsize);
    group.removeAnimation(&animrot);
}

void MovedCard::setRoute(QPoint p1, QPoint p2)
{
    animpos.setStartValue(p1);
    animpos.setEndValue(p2);
    posOn = true;
}

void MovedCard::setRotation(qreal ang1, qreal ang2)
{
    animrot.setStartValue(ang1);
    animrot.setEndValue(ang2);
    rotOn = true;
}

void MovedCard::setSize(qreal sx, qreal sy)
{
    animsize.setStartValue(sx);
    animsize.setEndValue(sy);
    sizeOn = true;
}

/*-----------------------------------------------------------------*/
CardDealer::CardDealer(QWidget* parent) : QWidget(parent)
{
    setWindowFlags(Qt::WindowTransparentForInput);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground,false);
    setWindowFlags(Qt::FramelessWindowHint);

    duration = 1000;
    timeline = new QTimeLine(duration, this);
    connect(this, &CardDealer::startMove, timeline, &QTimeLine::start);
    connect(timeline, &QTimeLine::finished, timeline, &QTimeLine::stop);
    connect(timeline, &QTimeLine::valueChanged, this, &CardDealer::timeChanged);
    connect(timeline, &QTimeLine::finished, this, &CardDealer::endMove);
    connect(this, &CardDealer::destroyed, timeline, &QTimeLine::deleteLater);
}

CardDealer::~CardDealer()
{
}

void CardDealer::append(const PixmapLabel &label, bool clearTable)
{
    MovedCard* card = new MovedCard(this, clearTable);
    QPoint p = label.mapToGlobal(QPoint(0, 0));
    p = this->mapFromGlobal(p);
    card->setScaledContents(false);
    card->setGeometry(QRect(p, label.size()));
    card->setStyleSheet(label.styleSheet());
    card->setRoute(p, clearTable ? bottomRightCorner : bottomLeftCorner);
    card->setSize(1, 0.1);
    card->setPixmap(clearTable ? label.getActPixmap().scaled(card->size()) : label.getBackPixmap().scaled(card->size()));
    if (!clearTable) card->setRotation(360, 0);
    cardlist.append(card);
}

void CardDealer::setDuration(int msec)
{
    duration = msec;
}

void CardDealer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    for (itCardList it = cardlist.begin(); it != cardlist.end(); it++)
    {
        MovedCard* card = *it;
        QPainter p(this);
        QTransform matrix;
        if (card->posOn)
        {
            QPointF p = card->animpos.currentValue().toPointF();
            matrix.translate(p.rx(), p.ry());
        }
        if (card->sizeOn)
        {
            qreal s = card->animsize.currentValue().toReal();
            matrix.scale(s, s);
        }
        if (card->rotOn)
        {
            qreal d = card->animrot.currentValue().toReal();
            matrix.rotate(d);
        }
        p.setTransform(matrix);
        card->render(&p, QPoint(0, 0));
    }
}

void CardDealer::resizeEvent(QResizeEvent *event)
{
    bottomRightCorner = QPoint(event->size().width(), event->size().height());
    bottomLeftCorner = QPoint(0, event->size().height());
}

void CardDealer::endMove()
{
    hide();
    foreach (MovedCard* card, cardlist) {
        delete card;
    }
    emit finished();
    cardlist.clear();
}

void CardDealer::timeChanged()
{
    repaint();
}

void CardDealer::start()
{
    foreach (MovedCard* card, cardlist) {
        card->animpos.setDuration(duration);
        card->animsize.setDuration(duration);
        card->animrot.setDuration(duration);
        card->animpos.start();
        card->animsize.start();
        card->animrot.start();
    }
    timeline->setDuration(duration);
    timeline->start();
    show();
    emit startMove();
}
