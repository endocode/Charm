#include <QtCore>

#include "RedmineConnector.h"

RedmineConnector::RedmineConnector(QObject *parent) :
    QObject(parent)
{
}

TaskList RedmineConnector::buildTaskList(QJsonArray projects, QJsonArray issues)
{
    TaskList tasks;
    std::transform(projects.begin(), projects.end(), std::back_inserter(tasks),
                   [this](const QJsonValue& v) { return parseProject(v.toObject()); } );
    return tasks;
}

Task RedmineConnector::parseProject(const QJsonObject &project)
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
