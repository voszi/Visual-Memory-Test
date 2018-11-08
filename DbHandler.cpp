#include "DbHandler.h"
#include <QSqlQuery>
#include <QSqlDriver>
#include <QSqlError>
#include <QFile>
#include <QDir>
#include <QDebug>
#include "TopList.h"

DbHandler* DbHandler::instance = nullptr;

const QString path("../database");
const QString dbname("VisualMemoryTest.sqlite");

DbHandler::DbHandler(QObject *parent)
    : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
}

DbHandler::~DbHandler()
{
    db.close();
    delete instance;
}

DbHandler *DbHandler::getDbHandler()
{
    if (!instance)
    {
        instance = new DbHandler;
        instance->setdb();
    }
    return instance;
}

bool DbHandler::setdb()
{
    bool create = false;
    QDir dir(path);
    QString fname = path+'/'+dbname;
    if (!QFile::exists(fname))
    {
        create = true;
        if (!dir.exists())
            dir.mkdir(path);
    }

    db.setDatabaseName(fname);
    db.open();
    if (create)
        createNewDb();
    return true;
}

bool DbHandler::createNewDb()
{
    /*
    QString player;
    QString bestResult;
    QString lastResult;
    QString bestTime;
    QString lastTime;     */

    QSqlQuery query;
    db.driver()->beginTransaction();
    bool ok = query.exec("CREATE TABLE SCORES "
                         "(NAME TEXT NOT NULL UNIQUE PRIMARY KEY, "
                         "BEST_RESULT INTEGER, "
                         "LAST_RESULT INTEGER, "
                         "BEST_TIME TEXT, "
                         "LAST_TIME TEXT)");
    if (!ok)
    {
        rollback(query.lastError());
        return false;
    }

    ok = query.exec("CREATE UNIQUE INDEX NAME_INDEX ON SCORES(NAME)");
    if (!ok)
    {
        rollback(query.lastError());
        return false;
    }

    db.driver()->commitTransaction();
    return true;
}

bool DbHandler::isEmpty()
{
    QSqlQuery query;
    db.open();
    query.exec("SELECT COUNT(*) FROM SCORES;");
    // driver doesn't support query.size()
    int rows = 0;
    if (query.next())
        rows = query.value(0).toInt();
    return 0 == rows;
}

void DbHandler::update(PlayerResult& res)
{
    QSqlQuery query;
    db.open();
    db.driver()->beginTransaction();
    query.prepare("SELECT * FROM SCORES WHERE NAME == ?");
    query.bindValue(0, res.player);
     if (!query.exec())
    {
        rollback(query.lastError());
        return;
    }
    if (query.next() && res.player == query.value(0).toString())
    {
        if (res.bestResult.toInt() < query.value(1).toInt())
            res.bestResult = query.value(1).toString();
        if (res.bestTime > query.value(3).toString())
            res.bestTime = query.value(3).toString();

        query.prepare("UPDATE SCORES "
                        "SET BEST_RESULT=:bestResult, LAST_RESULT=:lastResult, "
                      "BEST_TIME=:bestTime, LAST_TIME=:lastTime "
                      "WHERE NAME == :name");
    }
    else
    {
        query.prepare("INSERT INTO SCORES "
                      "(NAME, BEST_RESULT, LAST_RESULT, BEST_TIME, LAST_TIME) "
                      "VALUES (:name, :bestResult, :lastResult, :bestTime, :lastTime)");
    }
    query.bindValue(":name", res.player);
    query.bindValue(":bestResult", res.bestResult);
    query.bindValue(":lastResult", res.lastResult);
    query.bindValue(":bestTime", res.bestTime);
    query.bindValue(":lastTime", res.lastTime);
    if (!query.exec())
    {
        rollback(query.lastError());
        return;
    }
    db.driver()->commitTransaction();
    db.close();
}

void DbHandler::open()
{
    db.open();
}

QStringList DbHandler::getNames()
{
    QStringList list;
    QSqlQuery query("SELECT NAME FROM SCORES");
    if (query.exec())
    {
        while (query.next())
            list.append(query.value(0).toString());
    }
    return list;
}

void DbHandler::rollback(QSqlError error)
{
    Q_UNUSED(error)
    QSqlDatabase::database().driver()->rollbackTransaction();
}

void DbHandler::closeDb()
{
    db.close();
}



