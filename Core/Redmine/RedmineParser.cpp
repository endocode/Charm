#include "RedmineParser.h"

namespace Redmine {

namespace Parser {

Task parseProject(const QJsonObject &project)
{
    Task task;
    const QVariantMap projectData = project.toVariantMap();
    task.setId(projectData["id"].value<TaskId>());
    task.setName(projectData["name"].toString());
    QJsonObject parentObject = project["parent"].toObject();
    const QVariantMap parentData = parentObject.toVariantMap();
    task.setParent(parentData["id"].value<TaskId>());
    task.setValidFrom(QDateTime::fromString(projectData["created_on"].toString(), Qt::ISODate));
    return task;
}

Task parseIssue(const QJsonObject &issue)
{
    Task task;
    const QVariantMap issueData = issue.toVariantMap();
    QJsonObject projectObject = issue["project"].toObject();
    const QVariantMap projectData = projectObject.toVariantMap();
    const int project = projectData["id"].toInt();
    int taskIdBase = project;
    while (taskIdBase < 100000) taskIdBase *= 10;
    const int taskId = taskIdBase + issueData["id"].value<TaskId>();
    task.setId(taskId);
    task.setParent(project);
    task.setName(issueData["subject"].toString());
    task.setValidFrom(QDateTime::fromString(issueData["start_date"].toString(), Qt::ISODate));
    return task;
}

TaskList buildTaskList(QJsonArray projects, QJsonArray issues)
{
    TaskList tasks;
    std::transform(projects.begin(), projects.end(), std::back_inserter(tasks),
                   [](const QJsonValue& v) { return parseProject(v.toObject()); } );
    return tasks;
}

User parseUser(const QJsonObject &userJson)
{
    User user;
    QVariantMap userData = userJson.toVariantMap();
    user.setId(userData["id"].toInt());
    user.setName(QObject::tr("%1 %2").arg(userData["firstname"].toString(), userData["lastname"].toString()));
    return user;
}



}

}
