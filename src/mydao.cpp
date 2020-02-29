#include "mydao.h"
#include "api/color/colortable.h"

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

void MyDao::initGlobalSet()
{
    QSqlQuery qry;
    QString sql;
    sql = "select * from mind_global";
    sqliteWrapper->select(sql, qry);
    if (!qry.next()) {
        sql = QString("insert into mind_global(fontcolorindex,backcolorindex) values (%1,%2)").arg(0).arg(ColorTable::lastIndex());
        sqliteWrapper->execute(sql);
    }
    sql = "select * from mind_global";
    sqliteWrapper->select(sql, qry);
    if (qry.next()) {
        ColorTable::fontColorIndex = qry.value("fontcolorindex").toInt();
        ColorTable::backColorIndex = qry.value("backcolorindex").toInt();
    }
    //获取颜色
    sql = "select * from mind_color order by id desc";
    sqliteWrapper->select(sql, qry);
    int i = 0;
    while (qry.next()) {
        if (!ColorTable::colorIndexes.contains(qry.value("colorindex").toInt())) {
            ColorTable::colorIndexes.push(qry.value("colorindex").toInt());
            i++;
            if (i > 10) {
                break;
            }
        }
    }
}

void MyDao::saveGlobalSet(QString fldName, int value)
{
    QString sql = QString("update mind_global set %1=%2").arg(fldName).arg(value);
    sqliteWrapper->execute(sql);
}

void MyDao::addPix(int id, QPixmap& pix)
{
    QString sql = QString("update mind_data set img=? where id=%1").arg(id);
    QSqlQuery qry;
    sqliteWrapper->prepare(sql, qry);
    QByteArray ba;
    QBuffer buf(&ba);
    QImage img = pix.toImage();
    img.save(&buf, "png");
    qry.bindValue(0, ba);
    qry.exec();
}
