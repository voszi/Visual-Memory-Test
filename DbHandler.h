#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QObject>
#include <QSqlDatabase>
#include <QStringList>
#include "PlayerResult.h"

class DbHandler : public QObject
{
    Q_OBJECT
public:
    ~DbHandler();
    static DbHandler* getDbHandler();
    bool isEmpty();
    void update(PlayerResult& res);
    void open();
    QStringList getNames();

private:
    explicit DbHandler(QObject *parent = 0);
    static DbHandler* instance;
    QSqlDatabase db;
    void rollback(QSqlError error);
    bool setdb();
    bool createNewDb();

private slots:
    void closeDb();

};

#endif // DBHANDLER_H
