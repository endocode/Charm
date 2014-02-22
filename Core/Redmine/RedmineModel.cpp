#include <set>

#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>

#include <QVector>

#include <Core/User.h>
#include <Core/Task.h>
#include <Core/Redmine/IssueStatus.h>
#include <Core/Logging/Macros.h>

#include "RedmineModel.h"

namespace Redmine {

namespace {

auto compare_status_id = [](const Status& left, const Status& right) { return left.id() < right.id(); };
typedef std::set<Status, decltype(compare_status_id)> StatusSet;

auto compare_user_id = [](const User& left, const User& right) { return left.id() < right.id(); };
typedef std::set<User, decltype(compare_user_id)> UserSet;

}

#define LOCK_READ QReadLocker l(&d->lock_); Q_UNUSED(l);
#define LOCK_WRITE QWriteLocker l(&d->lock_); Q_UNUSED(l);

class Model::Private {
public:
    Private()
        : statuses_(StatusSet(compare_status_id))
        , users_(UserSet(compare_user_id))
    {}

    StatusSet statuses_;
    UserSet users_;
    User me_;
    TaskList tasks_;
    QReadWriteLock lock_;
};

Model::Model()
    : d(new Private())
{
}

const User &Model::currentUser() const
{
    LOCK_READ;
    return d->me_;
}

void Model::setCurrentUser(const User &me)
{
    LOCK_WRITE;
    d->me_ = me;
}

void Model::setUsers(const QVector<User> &users)
{
    LOCK_WRITE;
    d->users_.clear();
    std::copy(users.begin(), users.end(), std::inserter(d->users_, d->users_.begin()));
}

void Model::setIssueStatuses(const QVector<Status> &statuses)
{
    LOCK_WRITE;
    d->statuses_.clear();
    std::copy(statuses.begin(), statuses.end(), std::inserter(d->statuses_, d->statuses_.begin()));
}

void Model::appendTasks(const TaskList &tasks)
{
    LOCK_WRITE;
    d->tasks_.append(tasks);
}

TaskList Model::tasks() const
{
    LOCK_READ;
    return d->tasks_;
}

}
