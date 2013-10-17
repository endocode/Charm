#include <QtDebug>

#include <threadweaver/ThreadWeaver.h>
#include <threadweaver/JobPointer.h>
#include <threadweaver/Lambda.h>

#include <Core/Task.h>

#include "RedmineTaskListProvider.h"

#include "IssuesRetriever.h"
#include "ProjectsRetriever.h"


namespace Redmine {

template<typename T>
ThreadWeaver::JobPointer enqueue(ThreadWeaver::Weaver* weaver, T t) {
    ThreadWeaver::JobPointer ret(new ThreadWeaver::Lambda<T>(t));
    weaver->enqueue(ret);
    return ret;
}

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
    enqueue(ThreadWeaver::Weaver::instance(), [this]() { performUpdate(); } );
}

void TaskListProvider::performUpdate()
{
    qDebug() << "TaskListProvider::performUpdate: updating...";

    // Retrieve projects:
    Redmine::ProjectsRetriever projects(configuration_);
    projects.setWindow(0, 4);
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
    emit completed();
}

}

