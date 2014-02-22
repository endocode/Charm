#ifndef ISSUESRETRIEVER_H
#define ISSUESRETRIEVER_H

#include <threadweaver/Job.h>
#include <Core/Redmine/RedmineConfiguration.h>

#include "Core/Task.h"
#include "Core/User.h"
#include "Core/Redmine/WindowRetriever.h"

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
    QUrlQuery setupQuery() override;

private:
    TaskList issues_;
    User me_;
};

}

#endif // ISSUESRETRIEVER_H
