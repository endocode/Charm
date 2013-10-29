#include <threadweaver/ThreadWeaver.h>
#include <threadweaver/JobPointer.h>
#include <threadweaver/QObjectDecorator.h>
#include <threadweaver/JobCollection.h>
#include <threadweaver/JobSequence.h>
#include <threadweaver/ManagedJobPointer.h>
#include <threadweaver/Lambda.h>

#include <Core/Task.h>

#include "RedmineTaskListProvider.h"

#include "IssuesRetriever.h"
#include "ProjectsRetriever.h"
#include "RedmineUsersRetriever.h"
#include "CurrentUserRetriever.h"
#include "StatusRetriever.h"

#include <Core/Logging/Macros.h>

namespace ThreadWeaver {

template<typename T>
JobPointer enqueue(Weaver* weaver, T t) {
    JobPointer ret(new Lambda<T>(t));
    weaver->enqueue(ret);
    return ret;
}

template<typename T>
JobPointer enqueue(T t) {
    return enqueue(Weaver::instance(), t);
}

template<typename T>
JobPointer enqueueRaw(Weaver* weaver, T* t) {
    return ManagedJobPointer(t);
}

}

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
    ThreadWeaver::QObjectDecorator* sequence(new ThreadWeaver::QObjectDecorator(new ThreadWeaver::JobSequence()));
    // Phase 1: the jobs that need be be performed before the issues can be processed:
    QSharedPointer<ThreadWeaver::JobCollection> phase1(new ThreadWeaver::JobCollection());
    phase1->addJob(ThreadWeaver::JobPointer(new CurrentUserRetriever(model, configuration_)));
    phase1->addJob(ThreadWeaver::JobPointer(new UsersRetriever(model, configuration_)));
    phase1->addJob(ThreadWeaver::JobPointer(new StatusRetriever(model, configuration_)));

//    Collection phase1;
//    phase1 << new CurrentUserRetriever(model, configuration_)
//           << new UsersRetriever(model, configuration_)
//           << new StatusRetriever(model, configuration_);
//    sequence << phase1;

    // trackers
    // roles
    // project memberships
    // groups
    // Phase 2: mass-download of issues and projects:
    // ...
    // Phase 3: prepare task list for merge:
    // ...
    // ThreadWeaver::enqueue( [this]() { performUpdate(); } );
    sequence->sequence()->addJob(phase1);
    connect(sequence, SIGNAL(done(ThreadWeaver::JobPointer)), SIGNAL(completed()));
    connect(sequence, SIGNAL(failed(ThreadWeaver::JobPointer)), SIGNAL(error()));
    ThreadWeaver::Weaver::instance()->enqueue(ThreadWeaver::JobPointer(sequence));
}

void TaskListProvider::abortCurrentSynchronization()
{
    //FIXME we should keep a JobPointer to the currently executing sequence, and only requestAbort() this one:
    ThreadWeaver::Weaver::instance()->requestAbort();
}

void TaskListProvider::performUpdate()
{
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

