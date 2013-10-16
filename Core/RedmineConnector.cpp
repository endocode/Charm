#include <QtCore>

#include "CharmExceptions.h"
#include "RedmineConnector.h"

RedmineConnector::RedmineConnector(QObject *parent) :
    QObject(parent)
{
}

TaskList RedmineConnector::buildTaskListFromFile(const QString &filename)
{
    QFile inputFile(filename);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        throw CharmException(tr("Error opening file %1").arg(filename));
    }
    const QByteArray data = inputFile.readAll();
    QJsonParseError jsonParseError;
    const QJsonDocument jsonDoc =QJsonDocument::fromJson(data, &jsonParseError);
    if (jsonParseError.error != QJsonParseError::NoError) {
        throw CharmException(tr("Error parsing data from file %1").arg(filename));
    }
    QJsonObject json = jsonDoc.object();
    if (jsonDoc.isNull() || json.isEmpty()) {
        throw CharmException(tr("Document contains no data: %1").arg(filename));
    }
    QJsonArray projectsArray = json["projects"].toArray();

    TaskList tasks = buildTaskList(projectsArray, QJsonArray());
    return tasks;
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
