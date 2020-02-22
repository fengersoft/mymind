#ifndef MYDAO_H
#define MYDAO_H

#include "api/sql/sqlitewrapper.h"
#include <QApplication>
#include <QFileInfo>
#include <QObject>
#include <QSqlQuery>
#include <QStandardPaths>

class MyDao : public QObject {
    Q_OBJECT
public:
    explicit MyDao(QObject* parent = nullptr);
    SqliteWrapper* sqliteWrapper;
    int addNode(int pid, QString name, int sxh);
    void editNode(int id, QString name);
    ///初始化一些设置
    void initGlobalSet();
    void saveGlobalSet(QString fldName, int value);
signals:

public slots:
};

#endif // MYDAO_H
