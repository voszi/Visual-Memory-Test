#include "HelpDialog.h"
#include "ui_HelpDialog.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    if (e && e->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }

}

void HelpDialog::on_pushButton_clicked()
{
    close();
}
