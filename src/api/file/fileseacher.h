#ifndef FILESEACHER_H
#define FILESEACHER_H

#include <QDir>
#include <QObject>
#include <QtDebug>

class FileSeacher : public QObject {
    Q_OBJECT
public:
    explicit FileSeacher(QObject* parent = nullptr);

    void startSearch(QString rootPath);

private:
    QString rootPath;
    void searchDir(QString path);
signals:
    void findFileInfo(QString rootPath, QFileInfo& fileInfo);
public slots:
};

#endif // FILESEACHER_H
