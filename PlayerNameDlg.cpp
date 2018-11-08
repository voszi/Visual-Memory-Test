#include "PlayerNameDlg.h"
#include "ui_PlayerNameDlg.h"
#include "DbHandler.h"

PlayerNameDlg::PlayerNameDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayerNameDlg)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    ui->name->addItems(DbHandler::getDbHandler()->getNames());
    if (!ui->name->count())
        ui->btnOK->hide();
    ui->name->setEditText("");
 }

PlayerNameDlg::~PlayerNameDlg()
{
    delete ui;
}

QString PlayerNameDlg::getName()
{
    return ui->name->currentText();
}

void PlayerNameDlg::on_btnOK_pressed()
{
    accept();
}

void PlayerNameDlg::on_btnCancel_pressed()
{
    reject();
}

void PlayerNameDlg::on_name_currentTextChanged(const QString &arg1)
{
    ui->btnOK->setVisible(!arg1.isEmpty());
}
