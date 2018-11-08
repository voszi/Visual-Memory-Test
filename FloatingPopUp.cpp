#include "FloatingPopUp.h"
#include <QPainter>
#include <QDebug>

#define DURATION 3000
#define PAUSE 90*1000

FloatingPopUp::FloatingPopUp(QWidget *parent) : QWidget(parent)
{
    font = new QFont("MS Shell Dlg 2", 8, QFont::DemiBold);

    animopac = new QVariantAnimation(this);
    animopac->setStartValue(0.0);
    animopac->setEndValue(1.0);
    animopac->setDuration(DURATION);
    connect(animopac, &QVariantAnimation::finished, this, &FloatingPopUp::timeOut);
    connect(animopac, &QVariantAnimation::valueChanged, this, &FloatingPopUp::timeChanged);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FloatingPopUp::start);
    connect(this, &FloatingPopUp::destroyed, timer, &QTimer::deleteLater);
    direction = true;
    timer->start(PAUSE);
}

FloatingPopUp::~FloatingPopUp()
{
}

void FloatingPopUp::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    qreal opacity = animopac->currentValue().toReal();
    QPainter p(this);
    p.setOpacity(opacity);
    QColor bckclr("#ffbf00");
    QBrush brush = QBrush(bckclr);
    QPen pen = QPen(QColor(bckclr));
    p.setPen(pen);
    p.setBrush(brush);
    p.drawRoundedRect(0, 0, this->rect().width(), this->rect().height(), 6, 6);
    p.setPen(QColor(Qt::red));
    p.setFont(*font);
    p.drawText(this->rect(), Qt::AlignCenter, tr("Ez a program szerzői jogvédelem alá esik.\nKészítette: Voszkovenkó István"));
}

void FloatingPopUp::start()
{
    animopac->setDirection(direction ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
    animopac->start();
}

void FloatingPopUp::timeChanged()
{
    repaint();
}

void FloatingPopUp::timeOut()
{
    animopac->stop();
    if (direction)
    {
        direction = false;
        start();
    }
    else
    {
        direction = true;
    }
}

