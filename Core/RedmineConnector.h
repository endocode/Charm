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

Q_SIGNALS:
    
public Q_SLOTS:

private:
    friend class RedmineConnectorTests;
    TaskList buildTaskList(QJsonArray projects, QJsonArray issues);

    Task parseProject(const QJsonObject& project);
    Task parseIssue(const QJsonObject& issue);
};

#endif // REDMINECONNECTOR_H
