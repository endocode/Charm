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

Task parseIssue(const QJsonObject &issue, const User &me)
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
    if (me.isValid()) {
        QJsonObject ownerObject = issue["assigned_to"].toObject();
        const QVariantMap ownerData = ownerObject.toVariantMap();
        const int owner = ownerData["id"].toInt();
        task.setSubscribed(me.id() == owner);
    }
    return task;
}

User parseUser(const QJsonObject &userJson)
{
    User user;
    QVariantMap userData = userJson.toVariantMap();
    user.setId(userData["id"].toInt());
    user.setName(QObject::tr("%1 %2").arg(userData["firstname"].toString(), userData["lastname"].toString()));
    return user;
}

Status parseStatus(const QJsonObject &statusObject)
{
    Status status;
    QVariantMap statusData = statusObject.toVariantMap();
    status.setId(statusData["id"].value<Status::Id>());
    status.setName(statusData["name"].toString());
    status.setIsClosed(statusData.value("is_closed", false).toBool());
    status.setIsDefault(statusData.value("is_default", false).toBool());
    return status;
}



}

}
