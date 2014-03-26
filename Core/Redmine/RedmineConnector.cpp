#include <QtCore>

#include <ThreadWeaver/ThreadWeaver>

#include "CharmExceptions.h"
#include "RedmineConnector.h"
#include "RedmineParser.h"

#include <Core/Logging/Macros.h>

namespace Redmine {

Connector::Connector(QObject *parent)
    : QObject(parent)
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
    connect(&timer_, SIGNAL(timeout()), SLOT(triggerModelSynchronization()));
}

Connector::~Connector()
{
    timer_.stop();
    //FIXME requestAbort()
    ThreadWeaver::Queue::instance()->finish();
}

void Connector::triggerModelSynchronization()
{
    DEBUG("Connector::triggerModelSynchronization: starting model synchronization");
    Q_ASSERT(modelSynchronizer_ == 0);
    modelSynchronizer_.reset(new ModelDownloader(&configuration_));
    connect(modelSynchronizer_.data(), SIGNAL(completed()), SLOT(synchronizationCompleted()), Qt::QueuedConnection);
    connect(modelSynchronizer_.data(), SIGNAL(error()), SLOT(synchronizationAborted()), Qt::QueuedConnection);
    modelSynchronizer_->downloadModelData();
}

void Connector::synchronizationCompleted()
{
    DEBUG("Connector::updateCompleted: success.");
    emit updatedTaskList(modelSynchronizer_->model()->tasks());
    modelSynchronizer_.reset(0);
    timer_.setInterval(15 * 60 * 1000);
    handleUpdateFinished();
}

void Connector::synchronizationAborted()
{
    QString message = "I have no message for you yet, FIXME";
    DEBUG(tr("Connector::updateAborted: error: %1").arg(message));
    emit connectorError(message);
    modelSynchronizer_.reset(0);
    timer_.setInterval(60 * 60 * 1000);
    handleUpdateFinished();
}

void Connector::handleUpdateFinished()
{
    DEBUG(tr("Connector::handleUpdateFinished: next update in %1 minutes").arg(timer_.interval()/60000));
    timer_.start();
}

}
