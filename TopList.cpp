#include "TopList.h"
#include "ui_TopList.h"
#include <QString>
#include <QTableWidgetItem>
#include <QSqlQuery>
#include <QDebug>
#include "DbHandler.h"

TopList::TopList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TopList)
{
    qRegisterMetaType<PlayerResult>("PlayerResult");
    ui->setupUi(this);
    animation = new QPropertyAnimation(this, "size");
    model = new QSqlQueryModel(this);
    proxy= new QSortFilterProxyModel(this);
    connect(this, &TopList::destroyed, animation, &QPropertyAnimation::deleteLater);
    connect(this, &TopList::destroyed, proxy, &QSortFilterProxyModel::deleteLater);
    connect(this, &TopList::destroyed, model, &QSqlQueryModel::deleteLater);
    connect(ui->tableView->horizontalHeader(), &QHeaderView::sortIndicatorChanged, this, &TopList::sortByColumn);
    proxy->setSortLocaleAware(true);
    proxy->setSortCaseSensitivity(Qt::CaseInsensitive);
}

TopList::~TopList()
{
    delete ui;
}

void TopList::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    if (e && e->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
        setHeaders();
    }
}

void TopList::setHeaders()
{
    if (model)
    {
        model->setHeaderData(0, Qt::Horizontal, tr("Játékos"));
        model->setHeaderData(1, Qt::Horizontal, tr("Max. pont"));
        model->setHeaderData(2, Qt::Horizontal, tr("Akt. pont"));
        model->setHeaderData(3, Qt::Horizontal, tr("Legjobb idő"));
        model->setHeaderData(4, Qt::Horizontal, tr("Akt. idő"));
    }
}

void TopList::onSetPlayerResult(PlayerResult &res)
{
    DbHandler::getDbHandler()->update(res);
}

void TopList::show()
{
    DbHandler::getDbHandler()->open();
    model->setQuery("SELECT * FROM SCORES");
    proxy->setSourceModel(model);
    proxy->setDynamicSortFilter(true);
    ui->tableView->setModel(proxy);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(2, Qt::DescendingOrder);
    ui->tableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section {font: bold; color: white; background-color:darkCyan;}"
                "QHeaderView::up-arrow {image: url(:/images/up_arrow.png);}"
                "QHeaderView::down-arrow {image: url(:/images/down_arrow.png);}");
    ui->tableView->setStyleSheet(
                "QTableView {"
                "border-radius: 5px; color: darkBlue;"
                "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 #ffffff, stop:1 #0ad487);"
                "alternate-background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 #0ad487, stop:1 #ffffff)};");
    setHeaders();
    adjustSize();
    animation->setDuration(1500);
    animation->setStartValue(QSize(this->size().width(), 0));
    animation->setEndValue(this->size());
    animation->start();
    QWidget::show();
}

void TopList::sortByColumn(int col, Qt::SortOrder order)
{
    Q_UNUSED(order)
    switch (col)
    {
    case 0:
        ui->tableView->setLocale(parentWidget()->locale());
        ui->tableView->sortByColumn(col, order);
        break;
    default:
        setLocale(QLocale::system());
        break;
    }
}
