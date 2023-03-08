#ifndef PERSONALINFOENTRY_H
#define PERSONALINFOENTRY_H

#include <QString>
#include <QDate>

struct PersonalInfoEntry {
    int personal_info_id = -1;
    QString first_name;
    QString middle_name;
    QString last_name;
    QDate birth_date;
    QString nationality;
    QString address;
    QString email_address;
    QString phone_number;

    bool isValid() const
    {
        return !first_name.isEmpty() && !last_name.isEmpty() && birth_date.isValid() &&
               !nationality.isEmpty() && !address.isEmpty() &&
               !email_address.isEmpty() && !phone_number.isEmpty();
    }
};

#endif // PERSONALINFOENTRY_H
