#include "sqlitewrapper.h"

SqliteWrapper::SqliteWrapper(QObject* parent)
    : QObject(parent)
{
}

SqliteWrapper::SqliteWrapper(QString fileName, QString dbName)
{
    this->fileName = fileName;
    this->dbName = dbName;
}

void SqliteWrapper::setFileName(QString fileName)
{
    this->fileName = fileName;
}

void SqliteWrapper::setDbName(QString dbName)
{
    this->dbName = dbName;
}

void SqliteWrapper::open()
{

    database = QSqlDatabase::addDatabase("QSQLITE", dbName);
    database.setDatabaseName(fileName);
    opened = database.open();
}

void SqliteWrapper::close()
{
    database.close();
    opened = false;
}

bool SqliteWrapper::isOpened()
{
    return opened;
}

void SqliteWrapper::select(QString sql, QStandardItemModel* model)
{
    model->removeRows(0, model->rowCount());
    QSqlQuery qry(sql, database);
    while (qry.next()) {
        QList<QStandardItem*> items;
        for (int i = 0; i < qry.record().count(); i++) {
            QStandardItem* item = new QStandardItem();
            item->setText(qry.value(i).toString());
            items.append(item);
        }
        model->appendRow(items);
    }
}

void SqliteWrapper::select(QString sql, QSqlQuery& qry)
{
    qry = QSqlQuery(database);
    qry.exec(sql);
}

void SqliteWrapper::selectToTreeView(QString sql, int pid, QStandardItemModel* model)
{
    QSqlQuery qry(database);
    bool ret = qry.exec(sql);
    if (!ret) {
        qDebug() << sql;
        return;
    }
    QList<TreeviewData> items;
    while (qry.next()) {
        TreeviewData data;
        data.pid = qry.value(0).toInt();
        data.id = qry.value(1).toInt();
        data.text = qry.value(2).toString();
        for (int i = 0; i < qry.record().count(); i++) {
            data.values << qry.value(i).toString();
        }
        items << data;
    }
    model->clear();
    loadItemData(pid, model, nullptr, items);
}

void SqliteWrapper::loadItemData(int pid, QStandardItemModel* model, QStandardItem* parentItem, QList<TreeviewData>& items)
{
    for (int i = 0; i < items.count(); i++) {
        TreeviewData data = items.at(i);
        if (pid == data.pid) {
            QStandardItem* item = new QStandardItem();
            item->setText(data.text);
            for (int j = 0; j < data.values.count(); j++) {
                item->setData(data.values[j], Qt::UserRole + j + 1);
            }
            if (parentItem == nullptr) {
                model->appendRow(item);
            } else {
                parentItem->appendRow(item);
            }
            loadItemData(data.id, model, item, items);
        }
    }
}

int SqliteWrapper::findId(QString tableName, QString fieldName, QString fieldValue)
{
    QString sql = QString("select id from %1 where %2='%3'").arg(tableName).arg(fieldName).arg(fieldValue);
    QSqlQuery qry(database);
    bool ret = qry.exec(sql);
    if (!ret) {
        qDebug() << sql;
    }
    if (qry.next()) {
        return qry.value(0).toInt();
    } else {
        return -1;
    }
}

bool SqliteWrapper::execute(QString sql)
{
    QSqlQuery qry(database);
    bool ret = qry.exec(sql);
    if (!ret) {
        qDebug() << sql;
    }
    return ret;
}

void SqliteWrapper::truncateTables(QStringList& tables)
{
    for (int i = 0; i < tables.count(); i++) {
        QString sql = "delete from " + tables[i];
        execute(sql);
        sql = "DELETE FROM sqlite_sequence WHERE name = '" + tables[i] + "'";
        execute(sql);
    }
}

QSqlDatabase& SqliteWrapper::getDataBase()
{
    return database;
}
