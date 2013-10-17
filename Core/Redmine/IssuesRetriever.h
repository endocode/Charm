#ifndef ISSUESRETRIEVER_H
#define ISSUESRETRIEVER_H

#include <threadweaver/Job.h>
#include <Redmine/RedmineConfiguration.h>

#include "Task.h"
#include "RedmineRetriever.h"

namespace Redmine {

/** @brief IssuesRetriever extends Retriever to receive Redmine issues JSON data. */
class IssuesRetriever : public Retriever
{
public:
    IssuesRetriever(Configuration* config = 0);
    int offset() const { return offset_; }
    int limit() const { return limit_; }
    int total() const { return total_; }
    int count() const { return issues_.count(); }
    TaskList issues() const { return issues_; }
    void setWindow(int offset, int limit);

protected:
    QUrlQuery setupQuery() override;
    void run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread* thread);

private:
    TaskList issues_;
    int offset_, limit_, total_, requestOffset_, requestLimit_;
};

}

#endif // ISSUESRETRIEVER_H
