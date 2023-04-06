#include "databaseoperations.h"
#include <QMessageBox>

DatabaseOperations::DatabaseOperations(QObject *parent)
    : QObject{parent}
{

}

bool DatabaseOperations::connectToDatabase(QString dbPath)
{
    // Create file
    QFile file(dbPath);
    if(!file.exists())
    {
        file.open(QIODevice::OpenModeFlag::WriteOnly);
    }
    file.close();

    // Create database
    auto db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if(!db.open())
    {
        QMessageBox::critical(static_cast<QWidget*>(parent()), tr("Fatal Error"), tr("Failed to open the database file"));
        return false;
    }

    return true;
}

bool DatabaseOperations::InitializeDatabase()
{
    // Create tables
    QSqlQuery query;
    if(!query.exec("CREATE TABLE IF NOT EXISTS PersonalInfo "
            "(personal_info_id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "first_name TEXT NOT NULL, "
            "middle_name TEXT, "
            "last_name TEXT NOT NULL, "
            "birth_date INTEGER NOT NULL, "
            "nationality TEXT NOT NULL, "
            "address TEXT NOT NULL, "
            "email_address TEXT NOT NULL, "
            "phone_number TEXT NOT NULL, "
            "profile_photo TEXT)"))
        return false;

    if(!query.exec("CREATE TABLE IF NOT EXISTS WorkExperience "
            "(work_experience_id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "company_name TEXT NOT NULL, "
            "job_title TEXT NOT NULL, "
            "start_date INTEGER NOT NULL, "
            "end_date INTEGER NOT NULL, "
            "technologies_used TEXT NOT NULL, "
            "duties TEXT NOT NULL, "
            "location TEXT NOT NULL)"))
        return false;

    if(!query.exec("CREATE TABLE IF NOT EXISTS Education "
            "(education_id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "curse_name TEXT NOT NULL, "
            "school_name TEXT NOT NULL, "
            "location TEXT NOT NULL, "
            "start_date INTEGER NOT NULL, "
            "end_date INTEGER NOT NULL)"))
        return false;

    return true;
}

int DatabaseOperations::addEntry(Entry entry, EntryType entryType)
{
    bool result = false;
    QSqlQuery query;

    switch(entryType)
    {
        case EntryType::PERSONAL_INFO_ENTRY:
            if(query.prepare("INSERT INTO PersonalInfo (first_name, middle_name, last_name, birth_date, nationality, "
                          "address, email_address, phone_number, profile_photo) VALUES (:first_name, :middle_name, "
                          ":last_name, :birth_date, :nationality, :address, :email_address, :phone_number, :profile_photo)"))
            {
                auto item = std::get<PersonalInfoEntry>(entry);

                query.bindValue(":first_name", item.first_name);
                query.bindValue(":middle_name", item.middle_name);
                query.bindValue(":last_name", item.last_name);
                query.bindValue(":birth_date", item.birth_date.toJulianDay());
                query.bindValue(":nationality", item.nationality);
                query.bindValue(":address", item.address);
                query.bindValue(":email_address", item.email_address);
                query.bindValue(":phone_number", item.phone_number);
                query.bindValue(":profile_photo", item.profile_photo);

                result = query.exec();
            }
            break;
        case EntryType::WORK_EXPERIENCE_ENTRY:
            if(query.prepare("INSERT INTO WorkExperience (company_name, job_title, start_date, end_date, technologies_used, "
                              "duties, location) VALUES (:company_name, :job_title, :start_date, :end_date, :technologies_used, "
                              ":duties, :location)"))
            {
                auto item = std::get<WorkExperienceEntry>(entry);

                query.bindValue(":company_name", item.company_name);
                query.bindValue(":job_title", item.job_title);
                query.bindValue(":start_date", item.start_date.toJulianDay());
                query.bindValue(":end_date", item.end_date.toJulianDay());
                query.bindValue(":technologies_used", item.technologies_used);
                query.bindValue(":duties", item.duties);
                query.bindValue(":location", item.location);

                result = query.exec();
            }
            break;
        case EntryType::EDUCATION_ENTRY:
            if(query.prepare("INSERT INTO Education (curse_name, school_name, location, "
                              "start_date, end_date) VALUES (:curse_name, :school_name, :location, "
                              ":start_date, :end_date)"))
            {
                auto item = std::get<EducationEntry>(entry);

                query.bindValue(":curse_name", item.curse_name);
                query.bindValue(":school_name", item.school_name);
                query.bindValue(":location", item.location);
                query.bindValue(":start_date", item.start_date.toJulianDay());
                query.bindValue(":end_date", item.end_date.toJulianDay());

                result = query.exec();
            }
            break;
        default:
            break;
    }

    return result ? query.lastInsertId().toInt() : -1;
}

bool DatabaseOperations::updateEntry(Entry entry, EntryType entryType)
{
    bool result = false;
    QSqlQuery query;

    switch(entryType)
    {
        case EntryType::PERSONAL_INFO_ENTRY:
            if(query.prepare("UPDATE PersonalInfo SET first_name = :first_name, middle_name = :middle_name, "
                              "last_name = :last_name, birth_date = :birth_date, nationality = :nationality, "
                              "address = :address, email_address = :email_address, phone_number = :phone_number, "
                              "profile_photo = :profile_photo WHERE personal_info_id = :personal_info_id"))
            {
                auto item = std::get<PersonalInfoEntry>(entry);

                if(item.personal_info_id == -1)
                    return false;

                query.bindValue(":first_name", item.first_name);
                query.bindValue(":middle_name", item.middle_name);
                query.bindValue(":last_name", item.last_name);
                query.bindValue(":birth_date", item.birth_date.toJulianDay());
                query.bindValue(":nationality", item.nationality);
                query.bindValue(":address", item.address);
                query.bindValue(":email_address", item.email_address);
                query.bindValue(":phone_number", item.phone_number);
                query.bindValue(":profile_photo", item.profile_photo);

                query.bindValue(":personal_info_id", item.personal_info_id);

                result = query.exec();
            }
            break;
        case EntryType::WORK_EXPERIENCE_ENTRY:
            if(query.prepare("UPDATE WorkExperience SET company_name = :company_name, job_title = :job_title, "
                              "start_date = :start_date, end_date = :end_date, "
                              "technologies_used = :technologies_used, duties = :duties, location = :location"
                              " WHERE work_experience_id = :work_experience_id"))
            {
                auto item = std::get<WorkExperienceEntry>(entry);

                if(item.work_Experience_id == -1)
                    return false;

                query.bindValue(":company_name", item.company_name);
                query.bindValue(":job_title", item.job_title);
                query.bindValue(":start_date", item.start_date.toJulianDay());
                query.bindValue(":end_date", item.end_date.toJulianDay());
                query.bindValue(":technologies_used", item.technologies_used);
                query.bindValue(":duties", item.duties);
                query.bindValue(":location", item.location);

                query.bindValue(":work_experience_id", item.work_Experience_id);

                result = query.exec();
            }
            break;
        case EntryType::EDUCATION_ENTRY:
            if(query.prepare("UPDATE Education SET curse_name = :curse_name, school_name = :school_name, "
                              "location = :location, start_date = :start_date, end_date = :end_date"
                              " WHERE education_id = :education_id"))
            {
                auto item = std::get<EducationEntry>(entry);

                if(item.education_id == -1)
                    return false;

                query.bindValue(":curse_name", item.curse_name);
                query.bindValue(":school_name", item.school_name);
                query.bindValue(":location", item.location);
                query.bindValue(":start_date", item.start_date.toJulianDay());
                query.bindValue(":end_date", item.end_date.toJulianDay());

                query.bindValue(":education_id", item.education_id);

                result = query.exec();
            }
            break;
        default:
            break;
    }

    return result;
}

bool DatabaseOperations::removeEntry(Entry entry, EntryType entryType)
{
    bool result = false;
    QSqlQuery query;

    switch(entryType)
    {
        case EntryType::PERSONAL_INFO_ENTRY:
            if(query.prepare("DELETE FROM PersonalInfo WHERE first_name = :first_name AND "
                              "middle_name = :middle_name AND last_name = :last_name AND "
                              "birth_date = :birth_date AND nationality = :nationality AND "
                              "address = :address AND email_address = :email_address AND "
                              "phone_number = :phone_number AND profile_photo = :profile_photo AND "
                              "personal_info_id = :personal_info_id"))
            {
                auto item = std::get<PersonalInfoEntry>(entry);

                query.bindValue(":first_name", item.first_name);
                query.bindValue(":middle_name", item.middle_name);
                query.bindValue(":last_name", item.last_name);
                query.bindValue(":birth_date", item.birth_date.toJulianDay());
                query.bindValue(":nationality", item.nationality);
                query.bindValue(":address", item.address);
                query.bindValue(":email_address", item.email_address);
                query.bindValue(":phone_number", item.phone_number);
                query.bindValue(":profile_photo", item.profile_photo);

                query.bindValue(":personal_info_id", item.personal_info_id);

                result = query.exec();
            }
            break;
        case EntryType::WORK_EXPERIENCE_ENTRY:
            if(query.prepare("DELETE FROM WorkExperience WHERE company_name = :company_name AND "
                              "job_title = :job_title AND start_date = :start_date AND end_date = :end_date AND "
                              "technologies_used = :technologies_used AND duties = :duties AND"
                              " location = :location AND work_experience_id = :work_experience_id"))
            {
                auto item = std::get<WorkExperienceEntry>(entry);

                query.bindValue(":company_name", item.company_name);
                query.bindValue(":job_title", item.job_title);
                query.bindValue(":start_date", item.start_date.toJulianDay());
                query.bindValue(":end_date", item.end_date.toJulianDay());
                query.bindValue(":technologies_used", item.technologies_used);
                query.bindValue(":duties", item.duties);
                query.bindValue(":location", item.location);

                query.bindValue(":work_experience_id", item.work_Experience_id);

                result = query.exec();
            }
            break;
        case EntryType::EDUCATION_ENTRY:
            if(query.prepare("DELETE FROM Education WHERE curse_name = :curse_name AND "
                              "school_name = :school_name AND location = :location AND "
                              "start_date = :start_date AND end_date = :end_date AND education_id = :education_id"))
            {
                auto item = std::get<EducationEntry>(entry);

                query.bindValue(":curse_name", item.curse_name);
                query.bindValue(":school_name", item.school_name);
                query.bindValue(":location", item.location);
                query.bindValue(":start_date", item.start_date.toJulianDay());
                query.bindValue(":end_date", item.end_date.toJulianDay());

                query.bindValue(":education_id", item.education_id);

                result = query.exec();
            }
            break;
        default:
            break;
    }

   return result;
}

QList<DatabaseOperations::Entry> DatabaseOperations::getEntries(EntryType entryType)
{
    QList<DatabaseOperations::Entry> retVal;
    QSqlQuery query;

    switch(entryType)
    {
        case EntryType::PERSONAL_INFO_ENTRY:
            if(query.exec("SELECT personal_info_id, first_name, middle_name, last_name, birth_date, nationality, "
                           "address, email_address, phone_number, profile_photo FROM PersonalInfo"))
            {
                while(query.next())
                {
                    PersonalInfoEntry entry;

                    entry.personal_info_id = query.value(0).toInt();
                    entry.first_name = query.value(1).toString();
                    entry.middle_name = query.value(2).toString();
                    entry.last_name = query.value(3).toString();
                    entry.birth_date = QDate::fromJulianDay(query.value(4).toLongLong());
                    entry.nationality = query.value(5).toString();
                    entry.address = query.value(6).toString();
                    entry.email_address = query.value(7).toString();
                    entry.phone_number = query.value(8).toString();
                    entry.profile_photo = query.value(9).toString();

                    retVal.append(entry);
                }
            }
            break;
        case EntryType::WORK_EXPERIENCE_ENTRY:
            if(query.exec("SELECT work_experience_id, company_name, job_title, start_date, end_date, "
                           "technologies_used, duties, location FROM WorkExperience"))
            {
                while(query.next())
                {
                    WorkExperienceEntry entry;

                    entry.work_Experience_id = query.value(0).toInt();
                    entry.company_name = query.value(1).toString();
                    entry.job_title = query.value(2).toString();
                    entry.start_date = QDate::fromJulianDay(query.value(3).toLongLong());
                    entry.end_date = QDate::fromJulianDay(query.value(4).toLongLong());
                    entry.technologies_used = query.value(5).toString();
                    entry.duties = query.value(6).toString();
                    entry.location = query.value(7).toString();

                    retVal.append(entry);
                }
            }
            break;
        case EntryType::EDUCATION_ENTRY:
            if(query.exec("SELECT education_id, curse_name, school_name, location, "
                           "start_date, end_date FROM Education"))
            {
                while(query.next())
                {
                    EducationEntry entry;

                    entry.education_id = query.value(0).toInt();
                    entry.curse_name = query.value(1).toString();
                    entry.school_name = query.value(2).toString();
                    entry.location = query.value(3).toString();
                    entry.start_date = QDate::fromJulianDay(query.value(4).toLongLong());
                    entry.end_date = QDate::fromJulianDay(query.value(5).toLongLong());

                    retVal.append(entry);
                }
            }
            break;
        default:
            break;
    }

    return retVal;
}

int DatabaseOperations::totalEntries(EntryType entryType)
{
    QString sql = "SELECT COUNT(*) FROM ";

    switch(entryType)
    {
        case EntryType::PERSONAL_INFO_ENTRY:
        sql += "PersonalInfo";
        break;
        case EntryType::WORK_EXPERIENCE_ENTRY:
                sql += "WorkExperience";
                break;
        case EntryType::EDUCATION_ENTRY:
                sql += "Education";
                break;
        default:
            return -1;
    }

    QSqlQuery query;
    if(!query.exec(sql))
        return -1;

    query.first();

    return qvariant_cast<int>(query.value(0));
}
