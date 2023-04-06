#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>

#include "databaseoperations.h"
#include "pathhelpers.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "ResumeGenerator_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    if(!DatabaseOperations(nullptr).connectToDatabase(PathHelpers::combine({a.applicationDirPath(), "store.db"})))
    {
        QMessageBox::critical(nullptr, translator.tr("Fatal Error"), translator.tr("Failed to connect to a database"));
        return 1;
    }

    if(!DatabaseOperations(nullptr).InitializeDatabase())
    {
        QMessageBox::critical(nullptr, translator.tr("Fatal Error"), translator.tr("Failed to create a database schema"));
        return 1;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
