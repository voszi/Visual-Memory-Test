#include "BackGroundWidget.h"
#include "ui_BackGroundWidget.h"
#include <QPainter>
#include <QPoint>
#include <QRect>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QFont>
#include <QIcon>
#include <QDebug>
#include "PlayerNameDlg.h"
#include "DbHandler.h"


BackGroundWidget::BackGroundWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BackGroundWidget)
{
    initStrings();
    pixmap.load(":/images/sunflowers.jpg");
    graphics = new GraphicsWidget(this);
    timeFrame = new TimerWidget(this);
    btnstart = new QPushButton(this);
    btnstart->setStyleSheet("QPushButton{border-radius: 6px; background-color: #e9f3fb; color: #418fde;}"
                            "QPushButton:pressed {background-color: #66ccff; border-style: inset;}");
    btnstart->setText(txt_newplay);

    btnTopList = new QPushButton(this);
    btnTopList->setStyleSheet("QPushButton{border-bottom-right-radius: 4px; border-bottom-left-radius: 4px;"
                            "background-color: #1267A8; color: #e9f3fb;}"
                            "QPushButton:pressed {background-color: #66ccff; border-style: inset;}");
    btnTopList->setText(txt_toplist);
    if (!DbHandler::getDbHandler()->isEmpty())
        btnTopList->show();
    else
        btnTopList->hide();
    topList = new TopList(this);
    topList->setVisible(false);

    pixVoice[0].load(":/images/unmute.png");
    pixVoice[1].load(":/images/mute.png");
    btnVoice = new QPushButton(this);
    btnVoice->setStyleSheet("QPushButton{border-bottom-right-radius: 4px; border-bottom-left-radius: 4px;"
                                "background-color: #1267A8; color: #e9f3fb;}"
                                "QPushButton:pressed {background-color: #66ccff; border-style: inset;}");
    voiceOn = true;

    btnHelp = new QPushButton(this);
    btnHelp->setStyleSheet("QPushButton{border-bottom-right-radius: 4px; border-bottom-left-radius: 4px;"
                                "background-color: #1267A8; color: #e9f3fb;}"
                                "QPushButton:pressed {background-color: #66ccff; border-style: inset;}");
    btnHelp->setText("?");

    flpopup = new FloatingPopUp(this);

    connect(btnstart, &QPushButton::pressed, timeFrame, &TimerWidget::reset);
    connect(btnstart, &QPushButton::pressed, this, &BackGroundWidget::hideTopList);
    connect(btnstart, &QPushButton::pressed, this, &BackGroundWidget::onBtnStart);
    connect(this, &BackGroundWidget::startDeal, graphics, &GraphicsWidget::dealCard);
    connect(graphics, &GraphicsWidget::firstclicked, timeFrame, &TimerWidget::start);
    connect(graphics, &GraphicsWidget::result, timeFrame, &TimerWidget::setResult);
    connect(graphics, &GraphicsWidget::waitForPlayerDlg, this, &BackGroundWidget::onWaitForPlayerDlg);
    connect(graphics, &GraphicsWidget::finished, timeFrame, &TimerWidget::stop);
    connect(graphics, &GraphicsWidget::finished, this, &BackGroundWidget::onFinished);
    connect(this, &BackGroundWidget::setPlayerResult, topList, &TopList::onSetPlayerResult);
    connect(btnTopList, &QPushButton::pressed, this, &BackGroundWidget::showTopList);
    connect(btnVoice, &QPushButton::clicked, this, &BackGroundWidget::onBtnVoice);
    connect(btnHelp, &QPushButton::clicked, &help, &HelpDialog::show);

    dealer = new CardDealer(this);
    connect(graphics, &GraphicsWidget::clearTable, dealer, &CardDealer::start);
    connect(dealer, &CardDealer::finished, graphics, &GraphicsWidget::dealReady);

    // add flag bottoms
    languages << QLocale::Hungarian << QLocale::English << QLocale::German << QLocale::Spanish << QLocale::Russian;
    btnFlag = new QPushButton*[languages.size()];
    pixFlag = new QPixmap[static_cast<unsigned int>(languages.size())];
    //languages.
    frameFlags = new QFrame(this);
    flagsLayout = new QHBoxLayout(frameFlags);
    frameFlags->setStyleSheet("border-bottom-right-radius: 4px; border-bottom-left-radius: 4px;"
                            "background-color: #1267A8; color: #e9f3fb;");
    flagsLayout->setMargin(1);
    flagsLayout->setSpacing(2);
    grpFlags.setParent(this);
    grpFlags.setExclusive(true);
    for (int i = 0; i < languages.size(); i++)
    {
        QString flag = ":/images/flag_" + languages.at(i).name().left(2) + ".png";
        pixFlag[i].load(flag);
        btnFlag[i] = new QPushButton(frameFlags);
        btnFlag[i]->setCheckable(true);
        btnFlag[i]->setStyleSheet("border: no");
        grpFlags.addButton(btnFlag[i], i);
    }
    connect(&grpFlags, SIGNAL(buttonPressed(int)), this, SLOT(onFlagPressed(int)));

    // labels
    title.setParent(this);
    title.setText(txt_title);
    title.setStyleSheet("QLabel {color: white;}");
    madeBy.setParent(this);
    madeBy.setText(txt_madeby);
    madeBy.setStyleSheet("QLabel {color: white;}");

    // force load translator
    actLang = -1;
    btnFlag[1]->click();
}

BackGroundWidget::~BackGroundWidget()
{
    delete graphics;
    delete timeFrame;
    delete btnTopList;
    delete topList;
    delete btnstart;
    delete ui;
}

void BackGroundWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    painter.drawPixmap(pixrect, pixmap.scaled(pixrect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    graphics->show();
}

void BackGroundWidget::resizeEvent(QResizeEvent *e)
{
    QRect rect(0, 0, e->size().width(), e->size().height());
    graphics->setVisible(false);
    pixrect = QRect(QPoint(0,0), pixmap.size().scaled(e->size(), Qt::KeepAspectRatio));
    if (rect.width() > pixrect.width())
    {
        int shift = floor((rect.width()-pixrect.width())/2);
        pixrect.moveLeft(shift);
    }
    dealer->setGeometry(pixrect);
    dealer->show(); // trükkös!!!! :))
    dealer->hide();

    int w = pixrect.width()/3;
    QRect graphRect = QRect(0, 0, int(5*w/4), w);
    graphRect.moveCenter(pixrect.center());
    graphics->setGeometry(graphRect); // A többi buttont a graphRect alapján méretezzük

    // FloatingPopUp
    QRect flprect(0, 0, 250, 60);
    flprect.moveBottomLeft(QPoint(pixrect.bottomLeft().rx()+20, pixrect.bottomLeft().ry()-20));
    flpopup->setGeometry(flprect);

    // TimeFrame
    QRect boxrect = graphRect;
    boxrect.setHeight(graphRect.height()/8);
    QPoint point = graphRect.topLeft();
    boxrect.moveBottom(point.y()-10);
    timeFrame->setGeometry(boxrect);
    QRect labelrect = boxrect;  // ehhez igazítjuk majd a title-t

    // btnstart
    boxrect.setHeight(graphRect.height()/10);
    point = graphRect.bottomLeft();
    boxrect.moveTop(point.y()+10);
    btnstart->setGeometry(boxrect);
    QFont fontStart = btnstart->font();
    fontStart.setPixelSize(boxrect.height()/2);
    btnstart->setFont(fontStart);
    // Toplist
    boxrect.setWidth(boxrect.width()/8);
    boxrect.setHeight(2*boxrect.height()/3);
    boxrect.moveTopRight(QPoint(pixrect.topRight().x() - 2*boxrect.width(), pixrect.topRight().y()));
    btnTopList->setGeometry(boxrect);
    QFont fontTL = btnTopList->font();
    fontTL.setPixelSize(btnTopList->height()/2);
    btnTopList->setFont(fontTL);
    // Toplista
    QRect listrect = topList->rect();
    listrect.moveTopRight(btnTopList->geometry().bottomRight());
    topList->setGeometry(listrect);
    QRect flagrect = boxrect;   // for later use
    // btnVoice
    point = boxrect.topRight();
    boxrect.setWidth(boxrect.height());
    boxrect.moveTopLeft(QPoint(point.x() + boxrect.width()/2, point.y()));
    btnVoice->setGeometry(boxrect);
    btnVoice->setIconSize(btnVoice->size());
    btnVoice->setIcon(QIcon(pixVoice[0]));
    // btnHelp
    point = boxrect.topRight();
    boxrect.setWidth(boxrect.height());
    boxrect.moveTopLeft(QPoint(point.x() + boxrect.width()/2, point.y()));
    btnHelp->setGeometry(boxrect);
    btnHelp->setFont(fontTL);
    // Flag buttons
    flagrect.moveTo(QPoint(pixrect.left(), 0));
    flagrect.setWidth(flagrect.height());
    flagrect.moveLeft(pixrect.topLeft().x()+flagrect.width());
    // 4 button visible, that of langAct hidden
    frameFlags->setGeometry(flagrect.left(), flagrect.top(), 4 *(flagrect.width()+5)+2, flagrect.bottom()+4);

    for (int i = 0; i < languages.size(); i++)
    {
        btnFlag[i]->setParent(frameFlags);
        btnFlag[i]->setGeometry(flagrect);
        btnFlag[i]->setIconSize(flagrect.size());
        btnFlag[i]->setIcon(QIcon(pixFlag[i]));
        flagsLayout->addWidget(btnFlag[i]);
    }

    // Text labels Title & Developer
    labelrect.moveLeft(flagrect.topLeft().x());
    title.setGeometry(labelrect);
    QFont fontL = title.font();
    fontL.setPixelSize(3*flagrect.height()/4);
    title.setFont(fontL);
    title.setAlignment(Qt::AlignLeft|Qt::AlignTop);

    labelrect.moveTop(4*flagrect.bottomLeft().y());
    madeBy.setGeometry(labelrect);
    fontL = madeBy.font();
    fontL.setPixelSize(flagrect.height()/2);
    madeBy.setFont(fontL);
    madeBy.setAlignment(Qt::AlignLeft|Qt::AlignTop);

    setToolTips();
}

void BackGroundWidget::showTopList()
{
    if (topList->isVisible())
    {
        topList->hide();
        btnTopList->setText(txt_toplist);
        btnTopList->setToolTip(txt_openlist);
    }
    else
    {
        topList->show();
        btnTopList->setText(txt_close);
        btnTopList->setToolTip(txt_closelist);
    }
}

void BackGroundWidget::onFinished()
{
    PlayerNameDlg dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        PlayerResult res;
        res.player = dlg.getName();
        res.lastResult = timeFrame->getResult();
        res.lastTime = timeFrame->getTime();
        res.bestResult = timeFrame->getResult();
        res.bestTime = timeFrame->getTime();
        emit setPlayerResult(res);
        btnTopList->show();
    }
    btnstart->show();
}

void BackGroundWidget::onWaitForPlayerDlg()
{
    btnstart->hide();
}

void BackGroundWidget::hideTopList()
{
    topList->hide();
    btnTopList->setText(txt_toplist);
    btnTopList->setToolTip(txt_openlist);
}

void BackGroundWidget::onBtnVoice()
{
    voiceOn = !voiceOn;
    btnVoice->setIcon(QIcon(pixVoice[voiceOn ? 0 : 1]));
    graphics->setEffects(voiceOn);
    btnVoice->setToolTip(voiceOn ? txt_mute : txt_voiceon);
}

void BackGroundWidget::onBtnStart()
{
    emit startDeal(dealer);
}

void BackGroundWidget::onFlagPressed(int id)
{
    if (actLang != id)
    {
        QString fname = "VisualMemoryTest_" + languages[id].name() + ".qm";
        qApp->removeTranslator(&translator);
        translator.load(fname, "../languages");
        actLang = id;
        qApp->installTranslator(&translator);
        for (int i = 0; i < languages.size(); i++)
            btnFlag[i]->setVisible(i != actLang);
        initStrings();
        setLocale(languages[id]);
    }
}

void BackGroundWidget::initStrings()
{
    txt_close=tr("Bezárás");
    txt_newplay=tr("Új játék");
    txt_toplist=tr("Toplista");
    txt_help=tr("Segítség");
    txt_title=tr("Vizuális memória teszt");
    txt_madeby=tr("Készítette: Voszkovenkó István - 2018");
    txt_closelist=tr("Lista bezárása");
    txt_openlist=tr("Lista megnyitása");
    txt_mute=tr("Hang kikapcsolása");
    txt_voiceon=tr("Hang bekapcsolása");
    txt_hu=tr("magyar");
    txt_en=tr("angol");
    txt_de=tr("német");
    txt_es=tr("spanyol");
    txt_ru=tr("orosz");
}

void BackGroundWidget::changeEvent(QEvent *e)
{
    if (e)
    {
        if (e->type() == QEvent::LanguageChange)
        {
            ui->retranslateUi(this);
            // reset text
            btnstart->setText(txt_newplay);
            btnTopList->setText(topList->isVisible() ? txt_close : txt_toplist);
            title.setText(txt_title);
            madeBy.setText(txt_madeby);
            setToolTips();
        }
    }
    QWidget::changeEvent(e);
}

void BackGroundWidget::setToolTips()
{
    QStringList tips;
    tips << txt_hu << txt_en << txt_de << txt_es << txt_ru;
    for (int i = 0; i < languages.size(); i++)
        btnFlag[i]->setToolTip(tips.at(i));
    btnTopList->setToolTip(topList->isVisible() ? txt_closelist : txt_openlist);
    btnVoice->setToolTip(voiceOn ? txt_mute : txt_voiceon);
    btnHelp->setToolTip(txt_help);
}

