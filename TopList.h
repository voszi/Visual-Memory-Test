#ifndef TOPLIST_H
#define TOPLIST_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QPropertyAnimation>
#include <QSortFilterProxyModel>
#include "PlayerResult.h"

namespace Ui {
class TopList;
}

class TopList : public QWidget
{
    Q_OBJECT
public:
    explicit TopList(QWidget *parent = 0);
    ~TopList();
protected:
    void changeEvent(QEvent* e);
private:
    void setHeaders();
public slots:
    void onSetPlayerResult(PlayerResult& res);
    void show();
private slots:
    void sortByColumn(int col, Qt::SortOrder order);
private:
    Ui::TopList *ui;
    QSqlQueryModel* model;
    QSortFilterProxyModel* proxy;
    QPropertyAnimation* animation;
};

#endif // TOPLIST_H
