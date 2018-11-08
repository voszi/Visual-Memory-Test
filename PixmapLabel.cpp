#include "PixmapLabel.h"
#include <QDebug>
#include <QStylePainter>

QPixmap* PixmapLabel::backside = NULL;
QPixmap* PixmapLabel::emptypix = NULL;


PixmapLabel::PixmapLabel(QWidget *parent) :
    QLabel(parent)
{
    this->setMinimumSize(1,1);
    setScaledContents(false);
    setStyleSheet("QLabel{border-radius: 5px; background: transparent;}");

    animator = new QPropertyAnimation(this);
    animator->setTargetObject(this);
    animator->setStartValue(0.0);
    animator->setEndValue(1.0);
    animator->setDuration(300);
    loop = new QEventLoop(this);
    connect(animator, SIGNAL(valueChanged(const QVariant&)), SLOT(update()));
    connect(animator, SIGNAL(finished()), loop, SLOT(quit()));
    connect(animator, SIGNAL(finished()), SLOT(onAnimatorFinished()));
    connect(animator, SIGNAL(finished()), animator, SLOT(stop()));
}

PixmapLabel::~PixmapLabel()
{
    delete animator;
    delete loop;
}

int PixmapLabel::heightForWidth(int width) const
{
    return actpix->isNull() ? this->height() : ((qreal)pix.height()*width)/pix.width();
}

QSize PixmapLabel::sizeHint() const
{
    int w = this->width();
    return QSize(w, heightForWidth(w));
}

QPixmap PixmapLabel::scaledPixmap() const
{
    if (animator->state() == QAbstractAnimation::Running)
    {
        QRect rect = this->rect();
        QPoint c = rect.center();
        int w = rect.width();
        w *= animator->currentValue().toFloat();
        rect.setWidth(w);
        rect.moveCenter(c);
        return actpix->scaled(rect.size());
    }
    else
    {
        return actpix->scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
}

void PixmapLabel::loadBackSide(QString fname)
{
    backside = new QPixmap(fname);
    emptypix = new QPixmap(backside->size());
    emptypix->fill(QColor(0xffe9f3fb));
}

void PixmapLabel::flip()
{
    if (animator->state() != QAbstractAnimation::Stopped)
        loop->exec(QEventLoop::ExcludeUserInputEvents);
    reanimate = true;
    animator->setDirection(QAbstractAnimation::Backward);
    animator->start();
}

void PixmapLabel::setPixmap(const QPixmap &p, int id)
{
    pix = p;
    this->id = id;
    down = true;
    actpix = backside;
    QLabel::setPixmap(scaledPixmap());
}

void PixmapLabel::fill()
{
    actpix = emptypix;
}

/*
 * Ezt a fügvényt csak az újraosztás elején használjuk
 *
 */
void PixmapLabel::showBackSide()
{
    actpix = backside;
    QLabel::setPixmap(scaledPixmap());
}

void PixmapLabel::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e)
    if(!actpix->isNull())
        QLabel::setPixmap(scaledPixmap());
}

/*
 * Először leanimálja az aktuális pixmapet
 */
void PixmapLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton || !down || animator->state() != QAbstractAnimation::Stopped) return;
    emit cardClicked(this);
}

/*
 * flip után ez animálja fel az új oldalt
 */
void PixmapLabel::onAnimatorFinished()
{
    if (reanimate)
    {
        reanimate = false;
        if (down)
        {
            actpix = &pix;
            down = false;
        }
        else
        {
            actpix = backside;
            down = true;
        }
        animator->setDirection(QAbstractAnimation::Forward);
        animator->start();
    }
}

void PixmapLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPixmap pix = scaledPixmap();
    QSize size = this->size();
    int x = (size.width()-pix.width())/2;
    QPainter p(this);
    p.drawPixmap(x, 0, pix);
}
