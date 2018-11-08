#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QIcon>
#include <QDebug>
#include <QWindow>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Vizuális memória teszt v1.2 - ©:Voszkovenkó István"));
    setWindowState(Qt::WindowMaximized);
    card = new MovedCard(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete card;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape && windowState() == Qt::WindowFullScreen)
        setWindowState(Qt::WindowMaximized);
    else if ((e->key() == Qt::Key_F11 && windowState() != Qt::WindowFullScreen))
        setWindowState(Qt::WindowFullScreen);
    else
        QMainWindow::keyPressEvent(e);

}

void MainWindow::changeEvent(QEvent *e)
{
    if (e)
    {
        if (e->type() == QEvent::LanguageChange)
        {
            ui->retranslateUi(this);
            setWindowTitle(tr("Vizuális memória teszt v1.2 - ©:Voszkovenkó István"));
        }
    }
    QWidget::changeEvent(e);
}
