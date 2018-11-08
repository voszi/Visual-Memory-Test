#ifndef FLOATINGPOPUP_H
#define FLOATINGPOPUP_H

#include <QWidget>
#include <QVariantAnimation>
#include <QPaintEvent>
#include <QTimer>
#include <QFont>

class FloatingPopUp : public QWidget
{
    Q_OBJECT

public:
    explicit FloatingPopUp(QWidget *parent = 0);
    ~FloatingPopUp();
protected:
    void paintEvent(QPaintEvent *event);
private:
    QTimer* timer;
    QFont* font;
    QVariantAnimation* animopac;
    bool direction;
private slots:
    void start();
    void timeChanged();
    void timeOut();

};

#endif // FLOATINGPOPUP_H
