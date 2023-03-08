#ifndef EXPORTER_H
#define EXPORTER_H

#include <QObject>

#include "ItemTemplateInfo.h"

class Exporter : public QObject
{
    Q_OBJECT
public:
    explicit Exporter(QObject *parent = nullptr);
    bool exportAsPDF(QString path, QList<ItemTemplateInfo> infos);
};

#endif // EXPORTER_H
