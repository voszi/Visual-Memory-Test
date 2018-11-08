#ifndef PLAYERNAMEDLG_H
#define PLAYERNAMEDLG_H

#include <QDialog>
#include <QPropertyAnimation>

namespace Ui {
class PlayerNameDlg;
}

class PlayerNameDlg : public QDialog
{
    Q_OBJECT

public:
    explicit PlayerNameDlg(QWidget *parent = 0);
    ~PlayerNameDlg();
    QString getName();

private slots:
    void on_btnOK_pressed();
    void on_btnCancel_pressed();
    void on_name_currentTextChanged(const QString &arg1);

private:
    Ui::PlayerNameDlg *ui;
};

#endif // PLAYERNAMEDLG_H
