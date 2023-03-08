#ifndef DATABASEOPERATIONS_H
#define DATABASEOPERATIONS_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>

#include "PersonalInfoEntry.h"
#include "WorkExperienceEntry.h"
#include "EducationEntry.h"

enum class EntryType {
    PERSONAL_INFO_ENTRY,
    WORK_EXPERIENCE_ENTRY,
    EDUCATION_ENTRY
};

class DatabaseOperations : public QObject
{
    Q_OBJECT

public:
    using Entry = std::variant<PersonalInfoEntry, WorkExperienceEntry, EducationEntry>;
    explicit DatabaseOperations(QObject *parent = nullptr);
    bool connectToDatabase(QString dbPath);
    bool InitializeDatabase();
    int addEntry(Entry entry, EntryType entryType);
    bool updateEntry(Entry entry, EntryType entryType);
    bool removeEntry(Entry entry, EntryType entryType);
    QList<Entry> getEntries(EntryType entryType);
    int totalEntries(EntryType entryType);

signals:
};

#endif // DATABASEOPERATIONS_H
