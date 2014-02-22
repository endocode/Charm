#ifndef REDMINETASKLISTPROVIDER_H
#define REDMINETASKLISTPROVIDER_H

#include <QObject>
#include <QMutex>
#include <Core/Task.h>
#include <Core/Redmine/RedmineConfiguration.h>

namespace Redmine {

class Model;

//FIXME rename to ModelSynchronizer
class TaskListProvider : public QObject
{
    Q_OBJECT
public:
    explicit TaskListProvider(Configuration* config = 0, QObject *parent = 0);
    TaskList tasks() const;

    void synchronize(Model* model);


Q_SIGNALS:
    void completed();
    void error();

public Q_SLOTS:
    void abortCurrentSynchronization();

private:
    void verifyPhase1();
    void verifyPhase2();
    void verifyPhase3();
    Configuration* configuration_;
    TaskList tasks_;
    mutable QMutex mutex_;
};

}

#endif // REDMINETASKLISTPROVIDER_H
