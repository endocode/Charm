#include <threadweaver/ThreadWeaver.h>

#include <Core/Task.h>

#include "RedmineTaskListProvider.h"

#include "IssuesRetriever.h"
#include "ProjectsRetriever.h"
#include "RedmineUsersRetriever.h"
#include "CurrentUserRetriever.h"
#include "StatusRetriever.h"

#include <Core/Logging/Macros.h>

namespace Redmine {

TaskListProvider::TaskListProvider(Configuration *config, QObject *parent)
    : QObject(parent)
    , configuration_(config)
{
}

TaskList TaskListProvider::tasks() const
{
    QMutexLocker l(&mutex_);
    return tasks_;
}

void TaskListProvider::synchronize(Model* model)
{
    using namespace ThreadWeaver;

    // Phase 1: the jobs that need be be performed before the issues can be processed:
    Collection* phase1(new Collection());
    *phase1 << new CurrentUserRetriever(model, configuration_)
            << new UsersRetriever(model, configuration_)
            << new StatusRetriever(model, configuration_);
    // trackers
    // roles
    // project memberships
    // groups
    // Phase 2: mass-download of issues and projects:
    Collection* phase2(new Collection());
    *phase2 << make_job( [this]() { DEBUG(QObject::tr("This would be phase 2.")); } );
    // Phase 3: prepare task list for merge:
    Collection* phase3(new Collection());

    Sequence* sequence(new Sequence());
    *sequence
            << make_job( [this]() { DEBUG(QObject::tr("Initiating phase 1.")); } )
            << phase1
            << make_job( [this](){ verifyPhase1(); } )
            << phase2
            << make_job( [this](){ verifyPhase2(); } )
            << phase3
            << make_job( [this](){ verifyPhase3(); } )
            << make_job( [this]() { DEBUG(QObject::tr("Model synchronization complete.")); } );

    QObjectDecorator* job(new QObjectDecorator(sequence));
    connect(job, SIGNAL(done(ThreadWeaver::JobPointer)), SIGNAL(completed()));
    connect(job, SIGNAL(failed(ThreadWeaver::JobPointer)), SIGNAL(error()));
//    ThreadWeaver::Queueing::enqueue(&mutex_); // this does not (and should not) compile
    ThreadWeaver::enqueue(job);
}

void TaskListProvider::abortCurrentSynchronization()
{
    //FIXME we should keep a JobPointer to the currently executing sequence, and only requestAbort() this one:
    ThreadWeaver::Queue::instance()->requestAbort();
}

void TaskListProvider::verifyPhase1()
{
    DEBUG(QObject::tr("Phase 1 downloads completed, verifying..."));
}

void TaskListProvider::verifyPhase2()
{
    DEBUG(QObject::tr("Phase 2 finished, issues and projects downloaded. Building model."));
}

void TaskListProvider::verifyPhase3()
{
    DEBUG(QObject::tr("Phase 3 complete, updating model and handing over to application."));
//    DEBUG(tr("TaskListProvider::performUpdate: updating..."));
//    // Retrieve current user:
//    Redmine::CurrentUserRetriever currentUser(configuration_);
//    currentUser.blockingExecute();
//    if (!currentUser.success()) {
//        emit error("Error retrieving current user.");
//        return;
//    }
//    const User me = currentUser.currentUser();
//    DEBUG(tr("TaskListProvider::performUpdate: Current user: %1 (id %2)").arg(me.name()).arg(me.id()));

//    // Retrieve issue statuses:
//    Redmine::StatusRetriever statusRetriever(configuration_);
//    statusRetriever.blockingExecute();
//    if (!statusRetriever.success()) {
//        emit error("Error retrieving issue statuses.");
//        return;
//    }
//    DEBUG(tr("TaskListProvider::performUpdate: retrieved %1 issue statuses").arg(statusRetriever.statuses_().count()));

//    // Retrieve projects:
//    Redmine::ProjectsRetriever projects(configuration_);
//    projects.blockingExecute();
//    if (!projects.success()) {
//        emit error("Error retrieving projects.");
//        return;
//    }

//    TaskList tasks;
//    tasks << projects.projects();
//    if (projects.limit() < projects.total()) {
//        for(int current = projects.offset() + projects.limit(); current < projects.total(); current += projects.limit() ) {
//            Redmine::ProjectsRetriever chunk(configuration_);
//            chunk.setWindow(current, projects.limit());
//            chunk.blockingExecute();
//            if (!projects.success()) {
//                emit error("Error downloading projects.");
//                return;
//            }
//            tasks << chunk.projects();
//        }
//    }
//    DEBUG(tr("TaskListProvider::performUpdate: retrieved %1 total projects").arg(tasks.count()));

//    // Retrieve issues:
//    Redmine::IssuesRetriever issues(configuration_);
//    issues.setCurrentUser(me);
//    issues.blockingExecute();
//    if (!issues.success()) {
//        emit error("Error retrieving issues.");
//        return;
//    }

//    tasks << issues.issues();
//    if (issues.limit() < issues.total()) {
//        for(int current = issues.offset() + issues.limit(); current < issues.total(); current += issues.limit() ) {
//            Redmine::IssuesRetriever chunk(configuration_);
//            chunk.setCurrentUser(me);
//            chunk.setWindow(current, issues.limit());
//            chunk.blockingExecute();
//            if (!issues.success()) {
//                emit error("Error downloading issues.");
//                return;
//            }
//            tasks << chunk.issues();
//        }
//    }

//    DEBUG(tr("TaskListProvider::performUpdate: retrieved %1 total tasks").arg(tasks.count()));

//    {
//        QMutexLocker l(&mutex_);
//        tasks_ = tasks;
//    }
    //    emit completed();
}

}

