#include <QtDebug>

#include <threadweaver/ThreadWeaver.h>
#include <threadweaver/JobPointer.h>
#include <threadweaver/ManagedJobPointer.h>
#include <threadweaver/Lambda.h>

#include <Core/Task.h>

#include "RedmineTaskListProvider.h"

#include "IssuesRetriever.h"
#include "ProjectsRetriever.h"
#include "RedmineUsersRetriever.h"
#include "CurrentUserRetriever.h"

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

void TaskListProvider::update()
{
    ThreadWeaver::enqueue( [this]() { performUpdate(); } );
}

void TaskListProvider::performUpdate()
{
    qDebug() << "TaskListProvider::performUpdate: updating...";
    // Retrieve current user:
    Redmine::CurrentUserRetriever currentUser(configuration_);
    currentUser.blockingExecute();
    if (!currentUser.success()) {
        emit error("Error retrieving current user.");
        return;
    }
    const User me = currentUser.currentUser();
    qDebug() << "Current user:" << me.name() << "(id" << me.id() << ")";

    // Retrieve projects:
    Redmine::ProjectsRetriever projects(configuration_);
    projects.blockingExecute();
    if (!projects.success()) {
        emit error("Error retrieving projects.");
        return;
    }
    qDebug() << "Results: offset" << projects.offset() << "- limit" << projects.limit() << "- total" << projects.total();

    TaskList tasks;
    tasks << projects.projects();
    if (projects.limit() < projects.total()) {
        for(int current = projects.offset() + projects.limit(); current < projects.total(); current += projects.limit() ) {
            Redmine::ProjectsRetriever chunk(configuration_);
            chunk.setWindow(current, projects.limit());
            chunk.blockingExecute();
            if (!projects.success()) {
                emit error("Error downloading projects.");
                return;
            }
            tasks << chunk.projects();
        }
    }
    qDebug() << "Retrieved" << tasks.count() << "projects.";

    // Retrieve issues:
    Redmine::IssuesRetriever issues(configuration_);
    issues.setCurrentUser(me);
    issues.blockingExecute();
    if (!issues.success()) {
        emit error("Error retrieving issues.");
        return;
    }
    qDebug() << "Results: offset" << issues.offset() << "- limit" << issues.limit() << "- total" << issues.total();

    tasks << issues.issues();
    if (issues.limit() < issues.total()) {
        for(int current = issues.offset() + issues.limit(); current < issues.total(); current += issues.limit() ) {
            Redmine::IssuesRetriever chunk(configuration_);
            chunk.setCurrentUser(me);
            chunk.setWindow(current, issues.limit());
            chunk.blockingExecute();
            if (!issues.success()) {
                emit error("Error downloading issues.");
                return;
            }
            tasks << chunk.issues();
        }
    }

    {
        QMutexLocker l(&mutex_);
        tasks_ = tasks;
    }
    Q_FOREACH(Task task, tasks) {
        task.dump();
    }
    emit completed();
}

}

