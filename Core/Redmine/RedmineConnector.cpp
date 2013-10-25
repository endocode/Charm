#include <QtCore>

#include <threadweaver/ThreadWeaver.h>

#include "CharmExceptions.h"
#include "RedmineConnector.h"
#include "RedmineParser.h"

#include <Core/Logging/Macros.h>

namespace Redmine {

Connector::Connector(QObject *parent)
    : QObject(parent)
    , taskListProvider_(&configuration_)
{
    const QString server = qgetenv("CHARM_REDMINE_SERVER");
    if (server.isEmpty()) {
        throw CharmException("Environment variable CHARM_REDMINE_SERVER is not set!");
    }
    INFO(tr("Charm Redmine server: %1").arg(server));
    const QString apiKey = qgetenv("CHARM_REDMINE_APIKEY");
    if (apiKey.isEmpty()) {
        throw CharmException("Environment variable CHARM_REDMINE_APIKEY is not set!");
    }
    INFO(tr("Charm Redmine API key: %1").arg(apiKey));
    configuration_.setServer(QUrl(server));
    configuration_.setApiKey(apiKey);

    timer_.setSingleShot(true);
    timer_.setInterval(15 * 1000);
    timer_.start();
    connect(&timer_, SIGNAL(timeout()), &taskListProvider_, SLOT(update()));
    connect(&taskListProvider_, SIGNAL(completed()), SLOT(updateCompleted()), Qt::QueuedConnection);
    connect(&taskListProvider_, SIGNAL(error(QString)), SLOT(updateAborted(QString)), Qt::QueuedConnection);

}

Connector::~Connector()
{
    timer_.stop();
    ThreadWeaver::Weaver::instance()->finish();
}

void Connector::updateCompleted()
{
    DEBUG("Connector::updateCompleted: success.");
    emit updatedTaskList(taskListProvider_.tasks());
    timer_.setInterval(15 * 60 * 1000);
    handleUpdateFinished();
}

void Connector::updateAborted(QString message)
{
    DEBUG(tr("Connector::updateAborted: error: %1").arg(message));
    emit connectorError(message);
    timer_.setInterval(60 * 60 * 1000);
    handleUpdateFinished();
}

void Connector::handleUpdateFinished()
{
    DEBUG(tr("Connector::handleUpdateFinished: next update in %1 minutes").arg(timer_.interval()/60000));
    timer_.start();
}

}
