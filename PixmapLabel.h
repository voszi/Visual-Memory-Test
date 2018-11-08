#ifndef PIXMAPLABEL_H
#define PIXMAPLABEL_H

#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>
#include <QPropertyAnimation>
#include <QEventLoop>
#include <QColor>

class PixmapLabel : public QLabel
{
    Q_OBJECT
public:
    explicit PixmapLabel(QWidget* parent = 0);
    ~PixmapLabel();
    QPixmap scaledPixmap() const;
    static void loadBackSide(QString fname);
    int getId(){return id;}
    bool isDown(){return down;}
    void flip();
    void setPixmap (const QPixmap &p, int id);
    QPixmap getActPixmap() const {return *actpix;}
    QPixmap getBackPixmap() const {return *backside;}
    void fill();

public slots:
    void showBackSide();
private slots:
    void onAnimatorFinished();

protected:
    virtual int heightForWidth( int width ) const;
    virtual QSize sizeHint() const;
    void resizeEvent(QResizeEvent*e);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
signals:
    void cardClicked(PixmapLabel*);

private:
    static QPixmap* backside;
    static QPixmap* emptypix;
    QPropertyAnimation* animator;
    QEventLoop* loop;
    QPixmap pix;
    QPixmap* actpix;
    int id;
    bool down;
    bool reanimate;
};

#endif // PIXMAPLABEL_H
