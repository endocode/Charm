#ifndef ISSUESRETRIEVER_H
#define ISSUESRETRIEVER_H

#include <threadweaver/Job.h>
#include <Redmine/RedmineConfiguration.h>

#include "Task.h"
#include "WindowRetriever.h"

namespace Redmine {

/** @brief IssuesRetriever extends Retriever to receive Redmine issues JSON data. */
class IssuesRetriever : public WindowRetriever
{
public:
    IssuesRetriever(Configuration* config = 0);
    int count() const;
    TaskList issues() const;


protected:
    void run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread* thread) override;

private:
    TaskList issues_;
};

}

#endif // ISSUESRETRIEVER_H
