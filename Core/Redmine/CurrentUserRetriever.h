#ifndef CURRENTUSERRETRIEVER_H
#define CURRENTUSERRETRIEVER_H

#include <Core/User.h>

#include <Redmine/RedmineConfiguration.h>
#include <Redmine/RedmineRetriever.h>

namespace Redmine {

/** @brief Retrieve information about the current user from Redmine. */
class CurrentUserRetriever : public Retriever
{
public:
    CurrentUserRetriever(Configuration* config = 0);
    User currentUser() const;

protected:
    void run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread* thread);

private:
    User current_;
};

}

#endif // CURRENTUSERRETRIEVER_H
