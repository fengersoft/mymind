#include "fileseacher.h"

FileSeacher::FileSeacher(QObject* parent)
    : QObject(parent)
{
}

void FileSeacher::startSearch(QString rootPath)
{
    this->rootPath = rootPath;
    searchDir(rootPath);
}

void FileSeacher::searchDir(QString path)
{
    QDir dir(path);
    QFileInfo fileInfo;
    QFileInfoList fileList = dir.entryInfoList();
    for (int i = 0; i < fileList.count(); i++) {
        fileInfo = fileList.at(i);
        QString fileName = fileList.at(i).fileName().trimmed();
        if ((fileName == ".") || (fileName == "..")) {
            continue;
        }
        emit findFileInfo(rootPath, fileInfo);
        if (fileInfo.isDir()) {

            searchDir(fileInfo.filePath());
        }
    }
}
