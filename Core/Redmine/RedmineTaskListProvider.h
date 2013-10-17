#ifndef REDMINETASKLISTPROVIDER_H
#define REDMINETASKLISTPROVIDER_H

#include <QObject>
#include <QMutex>
#include <Core/Task.h>
#include <Core/Redmine/RedmineConfiguration.h>

namespace Redmine {

class TaskListProvider : public QObject
{
    Q_OBJECT
public:
    explicit TaskListProvider(Configuration* config = 0, QObject *parent = 0);
    TaskList tasks() const;

Q_SIGNALS:
    void completed();
    void error(QString);

public Q_SLOTS:
    void update();

private:
    void performUpdate();
    Configuration* configuration_;
    TaskList tasks_;
    mutable QMutex mutex_;
};

}

#endif // REDMINETASKLISTPROVIDER_H
