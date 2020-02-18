#ifndef SQLITEWRAPPER_H
#define SQLITEWRAPPER_H

#include "sqldef.h"
#include <QObject>
#include <QStandardItemModel>
#include <QtDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

class SqliteWrapper : public QObject {
    Q_OBJECT
public:
    explicit SqliteWrapper(QObject* parent = nullptr);
    SqliteWrapper(QString fileName, QString dbName);
    ///设置Sqlite文件路径
    void setFileName(QString fileName);
    void setDbName(QString dbName);
    void open();
    void close();
    bool isOpened();
    ///显示数据
    void select(QString sql, QStandardItemModel* model);
    void select(QString sql, QSqlQuery& qry);
    void selectToTreeView(QString sql, int pid, QStandardItemModel* model);
    void loadItemData(int pid, QStandardItemModel* model, QStandardItem* parentItem, QList<TreeviewData>& items);
    ///查找id
    int findId(QString tableName, QString fieldName, QString fieldValue);
    bool execute(QString sql);
    void truncateTables(QStringList& tables);
    QSqlDatabase& getDataBase();

private:
    QString fileName;
    QString dbName;
    QSqlDatabase database;
    bool opened;
signals:

public slots:
};

#endif // SQLITEWRAPPER_H
