#ifndef TIMERWIDGET_H
#define TIMERWIDGET_H

#include <QFrame>
#include <QHBoxLayout>
#include <QTimer>
#include <QLabel>

class TimerWidget : public QFrame
{
    Q_OBJECT
public:
    explicit TimerWidget(QWidget *parent = 0);
    ~TimerWidget();
    QString getResult(){return result->text();}
    QString getTime(){return elapsed->text();}
private:
    QHBoxLayout* box;
    QTimer timer;
    QLabel* elapsed;
    QLabel* result;
    int seconds;
    void showTime();
protected:
    void resizeEvent(QResizeEvent*e);
public slots:
    void start();
    void stop();
    void setResult(int val);
    void reset();
private slots:
    void onTimeOut();

};

#endif // TIMERWIDGET_H
