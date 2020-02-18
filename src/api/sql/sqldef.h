#ifndef SQLDEF_H
#define SQLDEF_H

#include <QObject>

struct TreeviewData {
    int pid;
    int id;
    QString text;
    QStringList values;
};
Q_DECLARE_METATYPE(TreeviewData);

#endif // SQLDEF_H
