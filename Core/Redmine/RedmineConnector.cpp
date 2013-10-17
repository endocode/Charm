#include <QtCore>

#include "CharmExceptions.h"
#include "RedmineConnector.h"
#include "RedmineParser.h"

namespace Redmine {

Connector::Connector(QObject *parent) :
    QObject(parent)
{
}

TaskList Connector::buildTaskListFromFile(const QString &filename)
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

    TaskList tasks = Parser::buildTaskList(projectsArray, QJsonArray());
    return tasks;
}

}
