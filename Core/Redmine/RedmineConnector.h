#ifndef REDMINECONNECTOR_H
#define REDMINECONNECTOR_H

#include <QObject>
#include <QTimer>

#include <Core/Task.h>
#include "RedmineConfiguration.h"
#include "RedmineTaskListProvider.h"

namespace Redmine {

class Connector : public QObject
{
    Q_OBJECT
public:
    explicit Connector(QObject *parent = 0);

    //temp, in lieu of a real API:
    TaskList buildTaskListFromFile(const QString& filename);

Q_SIGNALS:
    void updatedTaskList(TaskList);
    void connectorError(QString);

private Q_SLOTS:
    void updateCompleted();
    void updateAborted(QString);

private:
    void handleUpdateFinished();
    TaskListProvider taskListProvider_;
    QTimer timer_;
    Configuration configuration_;
};

}

#endif // REDMINECONNECTOR_H
