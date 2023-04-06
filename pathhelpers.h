#ifndef PATHHELPERS_H
#define PATHHELPERS_H

#include <QObject>

#include <initializer_list>

class PathHelpers : public QObject
{
    Q_OBJECT
public:
    static bool pathExists(QString path);
    static bool isAbsolutePath(QString path);
    static QString combine(std::initializer_list<QString> path);
    static QString getBasePath(QString path);
    static QString getFileName(QString path);
    static QString getFileExtension(QString path);
    static QString convertToNativeSeparators(QString path);
};

#endif // PATHHELPERS_H
