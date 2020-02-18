#include "mydao.h"

MyDao::MyDao(QObject* parent)
    : QObject(parent)
{
    sqliteWrapper = new SqliteWrapper(this);
#ifdef Q_OS_ANDROID
    QStringList paths = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
    QString path = paths[0] + "/data.db";
    QFileInfo info;
    if (!info.exists(path)) {

        QFile f;
        f.copy("assets:/data/data.db", path);
        f.setPermissions(path, QFile::ReadOwner | QFile::WriteOwner);
    }
    sqliteWrapper->setFileName(path);
#else
    sqliteWrapper->setFileName("data/data.db");
#endif

    sqliteWrapper->setDbName("data");
    sqliteWrapper->open();
}

int MyDao::addNode(int pid, QString name, int sxh)
{
    QString sql = QString("insert into mind_data(pid,name,sxh) values (%1,'%2',%3)").arg(pid).arg(name).arg(sxh);
    sqliteWrapper->execute(sql);
    QSqlQuery qry;
    sql = "select max(id) from mind_data";
    sqliteWrapper->select(sql, qry);
    qry.next();
    return qry.value(0).toInt();
}

void MyDao::editNode(int id, QString name)
{
    QString sql = QString("update mind_data set name='%1' where id=%2").arg(name).arg(id);
    sqliteWrapper->execute(sql);
}
