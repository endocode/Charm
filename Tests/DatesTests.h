#ifndef DATESTESTS_H
#define DATESTESTS_H

#include <QObject>

class DatesTests : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testDateByWeekNumberAndWorkDay();
    void testWeekDayInWeekOf();
    void testNumberOfWeeksInYear_data();
    void testNumberOfWeeksInYear();
};

#endif
