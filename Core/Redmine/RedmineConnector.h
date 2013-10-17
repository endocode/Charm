#ifndef REDMINECONNECTOR_H
#define REDMINECONNECTOR_H

#include <QObject>
#include <Core/Task.h>

namespace Redmine {

class Connector : public QObject
{
    Q_OBJECT
public:
    explicit Connector(QObject *parent = 0);
    
    //temp, in lieu of a real API:
    TaskList buildTaskListFromFile(const QString& filename);

    static Task parseProject(const QJsonObject& project);
    static Task parseIssue(const QJsonObject& issue);

    TaskList buildTaskList(QJsonArray projects, QJsonArray issues);

Q_SIGNALS:
    
public Q_SLOTS:

};

}

#endif // REDMINECONNECTOR_H
