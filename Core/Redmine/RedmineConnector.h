#ifndef REDMINECONNECTOR_H
#define REDMINECONNECTOR_H

#include <QObject>
#include <QTimer>
#include <QScopedPointer>

#include <Core/Redmine/RedmineModel.h>
#include <Core/Task.h>
#include "RedmineConfiguration.h"
#include "RedmineTaskListProvider.h"

namespace Redmine {

class Connector : public QObject
{
    Q_OBJECT
public:
    explicit Connector(QObject *parent = 0);
    ~Connector();

Q_SIGNALS:
    void updatedTaskList(TaskList);
    void connectorError(QString);

private Q_SLOTS:
    void synchronizationCompleted();
    void synchronizationAborted();

    void triggerModelSynchronization();

private:
    void handleUpdateFinished();

    QScopedPointer<ModelDownloader> modelSynchronizer_;
    QTimer timer_;
    Configuration configuration_;
};

}

#endif // REDMINECONNECTOR_H
