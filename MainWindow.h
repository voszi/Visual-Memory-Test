#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLocale>
#include "CardDealer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void keyPressEvent(QKeyEvent *e);
    void changeEvent(QEvent* e);
private:
    MovedCard* card;
    Ui::MainWindow *ui;
    QLocale actLocal;
signals:
    void changeLocal(QLocale local);
};

#endif // MAINWINDOW_H
