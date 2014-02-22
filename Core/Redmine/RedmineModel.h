#ifndef MODEL_H
#define MODEL_H

#include <Core/Task.h>
#include <Core/User.h>
#include <Core/Redmine/IssueStatus.h>

namespace Redmine {

/** @brief Redmine::Model holds the Redmine specific data for implementing Charm's data model. */
class Model
{
public:
    Model();

    const User& currentUser() const;

    void setCurrentUser(const User& me);
    void setUsers(const QVector<User>& users);
    void setIssueStatuses(const QVector<Status>& statuses);
    void appendTasks(const TaskList& tasks);

    TaskList tasks() const;
private:
    class Private;
    Private* const d;
};

}

#endif // MODEL_H
