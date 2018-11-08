#include "TimerWidget.h"
#include <QResizeEvent>
#include <QFont>
#include <QDebug>

TimerWidget::TimerWidget(QWidget *parent) : QFrame(parent)
{
    box = new QHBoxLayout(this);
    setStyleSheet("QFrame{border-radius: 10px; border-style: none; background-color: #e9f3fb;}");
    elapsed = new QLabel(this);
    elapsed->setStyleSheet("QLabel{background-color: #e9f3fb; color: #418fde;}");
    result = new QLabel(this);
    result->setStyleSheet("QLabel{background-color: #e9f3fb; color: #418fde;}");
    box->addWidget(elapsed); //, 0, Qt::AlignLeft|Qt::AlignVCenter);
    box->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
    box->addWidget(result, 0, Qt::AlignRight|Qt::AlignVCenter);
    setLayout(box);
    connect(&timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    elapsed->setText("00:00");
    result->setText("0");
}

TimerWidget::~TimerWidget()
{
    timer.stop();
    delete result;
    delete elapsed;
    delete box;
}

void TimerWidget::showTime()
{
    int min = floor(seconds/60);
    int sec = seconds-60*min;
    elapsed->setText(tr("%1:%2").arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0')));
}

void TimerWidget::resizeEvent(QResizeEvent *e)
{
    QFont font = elapsed->font();
    font.setPixelSize(e->size().height()/2);
    elapsed->setFont(font);
    result->setFont(font);
}

void TimerWidget::start()
{
    timer.start(1000);
    seconds = 0;
    result->setText("0");
}

void TimerWidget::stop()
{
    timer.stop();
}

void TimerWidget::setResult(int val)
{
    result->setText(tr("%1").arg(val));
}

void TimerWidget::reset()
{
    timer.stop();
    seconds = 0;
    showTime();
    setResult(0);
}

void TimerWidget::onTimeOut()
{
    seconds++;
    showTime();
}
