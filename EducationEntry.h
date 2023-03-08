#ifndef EDUCATIONENTRY_H
#define EDUCATIONENTRY_H

#include <QString>
#include <QDate>

struct EducationEntry {
    int education_id = -1;
    QString curse_name;
    QString school_name;
    QString location;
    QDate start_date;
    QDate end_date;

    bool isValid() const
    {
        return !curse_name.isEmpty() && !school_name.isEmpty() && !location.isEmpty() && start_date.isValid() && end_date.isValid();
    }
};

#endif // EDUCATIONENTRY_H
