#ifndef TEMPLATEPARSER_H
#define TEMPLATEPARSER_H

#include <QThread>

#include "ItemTemplateInfo.h"

class TemplateParser : public QThread
{
    Q_OBJECT
public:
    explicit TemplateParser(QObject *parent = nullptr);
    void run() override;
    void parse(QString file_path);

signals:
    void parsingSucceeded(QList<ItemTemplateInfo> infos);
    void parsingFailed(QString errorMsg);

private:
    void parseImpl();
    QString tmpl_path;
};

#endif // TEMPLATEPARSER_H
