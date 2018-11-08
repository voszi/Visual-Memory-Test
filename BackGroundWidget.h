#ifndef BACKGROUNDWIDGET_H
#define BACKGROUNDWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QGraphicsView>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include "GraphicsWidget.h"
#include <QThread>
#include <QTranslator>
#include <QLocale>
#include <QList>
#include "TimerWidget.h"
#include "TopList.h"
#include "CardDealer.h"
#include "FloatingPopUp.h"
#include "HelpDialog.h"

namespace Ui {
class BackGroundWidget;
}

class BackGroundWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BackGroundWidget(QWidget *parent = 0);
    ~BackGroundWidget();
protected:
    void paintEvent(QPaintEvent* e);
    void resizeEvent(QResizeEvent *e);
    void changeEvent(QEvent* e);
private:
    void setToolTips();
private slots:
    void showTopList();
    void onFinished();
    void onWaitForPlayerDlg();
    void hideTopList();
    void onBtnVoice();
    void onBtnStart();
    void onFlagPressed(int id);
signals:
    void setPlayerResult(PlayerResult& res);
    void startDeal(CardDealer* dealer);

private:
    QString txt_close;
    QString txt_newplay;
    QString txt_toplist;
    QString txt_help;
    QString txt_title;
    QString txt_madeby;
    QString txt_closelist;
    QString txt_openlist;
    QString txt_mute;
    QString txt_voiceon;
    QString txt_hu;
    QString txt_en;
    QString txt_de;
    QString txt_es;
    QString txt_ru;
    void initStrings();
    Ui::BackGroundWidget *ui;
    QPixmap pixmap;
    QRect pixrect;
    GraphicsWidget *graphics;
    TimerWidget* timeFrame;
    QPushButton* btnstart;
    QPushButton* btnTopList;
    QPushButton* btnVoice;
    QPushButton* btnHelp;
    TopList* topList;
    QPixmap pixVoice[2];
    QButtonGroup grpFlags;
    QFrame* frameFlags;
    QLayout* flagsLayout;
    QPushButton** btnFlag;
    QPixmap* pixFlag;
    QList<QLocale> languages;
    int actLang;
    bool voiceOn;
    CardDealer* dealer;
    FloatingPopUp* flpopup;
    QTranslator translator;
    QLabel title;
    QLabel madeBy;
    HelpDialog help;

};

#endif // BACKGROUNDWIDGET_H
