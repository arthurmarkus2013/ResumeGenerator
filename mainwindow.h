#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>

#include "workexperiencedatamodel.h"
#include "educationdatamodel.h"

#include "templateparser.h"
#include "exporter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAddWorkExperienceEntry_clicked();

    void on_btnAddEducationEntry_clicked();

    void on_btnRemoveWorkExperienceEntry_clicked();

    void on_btnRemoveEducationEntry_clicked();

    void on_actionChoose_Template_triggered();

    void on_action_Export_As_PDF_triggered();

    void on_actionE_xit_triggered();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void timerTriggered();
    void persistValidChanges();
    void populateStatusBar();
    void updateInfos(QList<ItemTemplateInfo> infos);
    void reportParsingError(QString msg);
    void fetchPersonalInfo();
    Ui::MainWindow *ui;
    WorkExperienceDataModel *weModel;
    EducationDataModel *eduModel;
    QTimer *timer;
    DatabaseOperations *db;
    QList<ItemTemplateInfo> infos;
    TemplateParser *tmplParser;
    Exporter *exporter;
    QLabel *lblTepmplatePath;
    QLabel *lblCurrentStatus;
};
#endif // MAINWINDOW_H
