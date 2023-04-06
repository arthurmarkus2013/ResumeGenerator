#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fetchPersonalInfo();

    populateStatusBar();

    exporter = new Exporter(this);

    tmplParser = new TemplateParser(this);
    connect(tmplParser, &TemplateParser::parsingSucceeded, this, &MainWindow::updateInfos);
    connect(tmplParser, &TemplateParser::parsingFailed, this, &MainWindow::reportParsingError);

    db = new DatabaseOperations(this);

    weModel = new WorkExperienceDataModel();
    ui->tableWorkExperience->setModel(weModel);

    eduModel = new EducationDataModel();
    ui->tableEducation->setModel(eduModel);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::timerTriggered);
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    timer->stop();

    delete lblTepmplatePath;
    delete lblCurrentStatus;
    delete weModel;
    delete eduModel;
    delete timer;
    delete db;
    delete tmplParser;
    delete exporter;
    delete ui;
}

void MainWindow::fetchPersonalInfo()
{
    if(db->totalEntries(EntryType::PERSONAL_INFO_ENTRY) == 0)
        return;

    auto entry = std::get<PersonalInfoEntry>(db->getEntries(EntryType::PERSONAL_INFO_ENTRY).at(0));

    ui->editFirstName->setText(entry.first_name);
    ui->editMiddleName->setText(entry.middle_name);
    ui->editLastName->setText(entry.last_name);
    ui->editBirthDate->setDate(entry.birth_date);
    ui->editAddress->setText(entry.address);
    ui->editEmailAddress->setText(entry.email_address);
    ui->editNationality->setText(entry.nationality);
    ui->editPhoneNumber->setText(entry.phone_number);
    ui->editProfilePhoto->setText(entry.profile_photo);
}

void MainWindow::timerTriggered()
{
    lblCurrentStatus->setText(tr("Persisting valid data..."));
    persistValidChanges();
    weModel->persistValidChanges();
    eduModel->persistValidChanges();
    lblCurrentStatus->setText(tr("Ready"));
}

void MainWindow::persistValidChanges()
{
    PersonalInfoEntry entry;
    hasInvalidData = false;

    if(db->totalEntries(EntryType::PERSONAL_INFO_ENTRY) > 0)
        entry = std::get<PersonalInfoEntry>(db->getEntries(EntryType::PERSONAL_INFO_ENTRY).at(0));

    entry.first_name = ui->editFirstName->text();
    entry.middle_name = ui->editMiddleName->text();
    entry.last_name = ui->editLastName->text();
    entry.birth_date = ui->editBirthDate->date();
    entry.address = ui->editAddress->text();
    entry.email_address = ui->editEmailAddress->text();
    entry.nationality = ui->editNationality->text();
    entry.phone_number = ui->editPhoneNumber->text();
    entry.profile_photo = ui->editProfilePhoto->text();

    if(entry.isValid())
    {
        if(!db->updateEntry(entry, EntryType::PERSONAL_INFO_ENTRY))
            entry.personal_info_id = db->addEntry(entry, EntryType::PERSONAL_INFO_ENTRY);
    }
    else
        hasInvalidData = true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    lblCurrentStatus->setText(tr("Exiting... Please wait..."));

    persistValidChanges();
    weModel->persistValidChanges();
    eduModel->persistValidChanges();

    if(hasInvalidData || weModel->hasInvalidData() || eduModel->hasInvalidData())
    {
        auto result = QMessageBox::question(this, tr("Warning"),
                                            tr("Do you really wanna exit? You have some unfinished entries, that won't be persisted!"));

        event->setAccepted(result == QMessageBox::StandardButton::Yes);
    }
}

void MainWindow::populateStatusBar()
{
    lblCurrentStatus = new QLabel(ui->statusBar);
    lblTepmplatePath = new QLabel(ui->statusBar);

    lblCurrentStatus->setText(tr("Ready"));
    lblTepmplatePath->setText(tr("Selected Template: (none)"));

    ui->statusBar->insertWidget(0, lblCurrentStatus, 100);
    ui->statusBar->insertWidget(1, lblTepmplatePath, 250);
}

void MainWindow::reportParsingError(QString msg)
{
    lblCurrentStatus->setText(tr("Templlate parsing failed..."));
    lblTepmplatePath->setText(tr("Selected Template: (none)"));

    QMessageBox::critical(this, tr("Parsing Error"), msg);
}

void MainWindow::updateInfos(QList<ItemTemplateInfo> infos)
{
    this->infos = infos;

    ui->action_Export_As_PDF->setEnabled(true);

    lblCurrentStatus->setText(tr("Template parsing succeeded..."));
}

void MainWindow::on_btnAddWorkExperienceEntry_clicked()
{
    ui->tableWorkExperience->model()->insertRow(ui->tableWorkExperience->model()->rowCount());
}


void MainWindow::on_btnAddEducationEntry_clicked()
{
    ui->tableEducation->model()->insertRow(ui->tableEducation->model()->rowCount());
}


void MainWindow::on_btnRemoveWorkExperienceEntry_clicked()
{
    if(ui->tableWorkExperience->selectionModel()->hasSelection())
        ui->tableWorkExperience->model()->removeRow(ui->tableWorkExperience->selectionModel()->selectedIndexes().at(0).row());
}


void MainWindow::on_btnRemoveEducationEntry_clicked()
{
    if(ui->tableEducation->selectionModel()->hasSelection())
        ui->tableEducation->model()->removeRow(ui->tableEducation->selectionModel()->selectedIndexes().at(0).row());
}


void MainWindow::on_actionChoose_Template_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose Resume Template"),
                                                    QStandardPaths::locate(QStandardPaths::StandardLocation::DocumentsLocation, ""),
                                                    "Resume Generator Template File (*.rgtf)");
    lblTepmplatePath->setText(tr("Selected Template: ") + filename);

    if(!filename.isEmpty())
    {
        ui->action_Export_As_PDF->setEnabled(false);
        lblCurrentStatus->setText(tr("Parsing template..."));
        tmplParser->parse(filename);
    }
}


void MainWindow::on_action_Export_As_PDF_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Export As PDF"),
                                                    QStandardPaths::locate(QStandardPaths::StandardLocation::DocumentsLocation, ""),
                                                    "Portable Document Format (*.pdf)");

    if(!filename.isEmpty())
    {
        lblCurrentStatus->setText(tr("Exporting as PDF..."));
        exporter->exportAsPDF(filename, infos);
        lblCurrentStatus->setText(tr("Ready"));
    }
}


void MainWindow::on_actionE_xit_triggered()
{
    this->close();
}


void MainWindow::on_btnFindProfilePhoto_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Find Profile Photo"),
                                                    QStandardPaths::locate(QStandardPaths::StandardLocation::PicturesLocation, ""),
                                                    "JPEG File (*.jpg *.jpeg)");

    if(!filename.isEmpty())
    {
        ui->editProfilePhoto->setText(filename);
    }
}

