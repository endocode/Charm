#ifndef REDMINETASKLISTPROVIDER_H
#define REDMINETASKLISTPROVIDER_H

#include <QObject>
#include <QMutex>

#include <ThreadWeaver/Collection>
#include <ThreadWeaver/Sequence>

#include <Core/Task.h>
#include <Core/Redmine/RedmineModel.h>
#include <Core/Redmine/RedmineConfiguration.h>

namespace Redmine {

class Model;

//FIXME rename to ModelSynchronizer
class TaskListProvider : public QObject
{
    Q_OBJECT
public:
    explicit TaskListProvider(Configuration* config = 0, QObject *parent = 0);
    void downloadModelData();
    Model* model();

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

    Model model_;
    ThreadWeaver::Collection phase1_;
    ThreadWeaver::Collection phase2_;
    ThreadWeaver::Collection phase3_;
    ThreadWeaver::Sequence sequence_;
};

}

#endif // REDMINETASKLISTPROVIDER_H
