#ifndef ISSUESRETRIEVER_H
#define ISSUESRETRIEVER_H

#include <threadweaver/Job.h>
#include <Redmine/RedmineConfiguration.h>

#include "Task.h"
#include "User.h"
#include "WindowRetriever.h"

namespace Redmine {

/** @brief IssuesRetriever extends WindowRetriever to receive Redmine issues JSON data. */
class IssuesRetriever : public WindowRetriever
{
public:
    IssuesRetriever(Configuration* config = 0);
    void setCurrentUser(const User& user);
    User currentUser() const;

    int count() const;
    TaskList issues() const;


protected:
    void run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread* thread) override;

private:
    TaskList issues_;
    User me_;
};

}

#endif // ISSUESRETRIEVER_H
