#ifndef MODEL_H
#define MODEL_H

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

private:
    class Private;
    Private* const d;
};

}

#endif // MODEL_H
