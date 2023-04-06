#include "pathhelpers.h"

#include <QDir>

bool PathHelpers::pathExists(QString path)
{
    QDir tmp(path);
    return tmp.exists();
}

bool PathHelpers::isAbsolutePath(QString path)
{
    return QDir::isAbsolutePath(path);
}

QString PathHelpers::combine(std::initializer_list<QString> path)
{
    QString retVal;

    for(auto &item : path)
    {
        retVal += item + "/";
    }

    return retVal.removeAt(retVal.size() - 1);
}

QString PathHelpers::getBasePath(QString path)
{
    return path.mid(0, path.lastIndexOf("/"));
}

QString PathHelpers::getFileName(QString path)
{
    return path.mid(path.lastIndexOf("/"));
}

QString PathHelpers::getFileExtension(QString path)
{
    return path.mid(path.lastIndexOf(".") + 1);
}

QString PathHelpers::convertToNativeSeparators(QString path)
{
    return QDir::toNativeSeparators(path).replace("\\", "\\\\");
}
