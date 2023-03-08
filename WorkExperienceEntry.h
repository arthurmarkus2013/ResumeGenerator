#ifndef WORKEXPERIENCEENTRY_H
#define WORKEXPERIENCEENTRY_H

#include <QString>
#include <QDate>

struct WorkExperienceEntry {
    int work_Experience_id = -1;
    QString company_name;
    QString job_title;
    QDate start_date;
    QDate end_date;
    QString technologies_used;
    QString duties;
    QString location;

    bool isValid() const
    {
        return !company_name.isEmpty() && !job_title.isEmpty() &&
               start_date.isValid() && end_date.isValid() && !technologies_used.isEmpty()
               && !duties.isEmpty() && !location.isEmpty();
    }
};

#endif // WORKEXPERIENCEENTRY_H
