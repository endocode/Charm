#ifndef REDMINECONNECTOR_H
#define REDMINECONNECTOR_H

#include <QObject>
#include "Task.h"

class RedmineConnector : public QObject
{
    Q_OBJECT
public:
    explicit RedmineConnector(QObject *parent = 0);
    
    //temp, in lieu of a real API:
    TaskList buildTaskListFromFile(const QString& filename);

    static Task parseProject(const QJsonObject& project);
    static Task parseIssue(const QJsonObject& issue);

Q_SIGNALS:
    
public Q_SLOTS:

private:
    friend class RedmineConnectorTests;
    TaskList buildTaskList(QJsonArray projects, QJsonArray issues);

};

#endif // REDMINECONNECTOR_H
