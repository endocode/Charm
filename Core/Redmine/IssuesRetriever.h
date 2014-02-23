#ifndef ISSUESRETRIEVER_H
#define ISSUESRETRIEVER_H

#include <threadweaver/Job.h>
#include <Core/Redmine/RedmineConfiguration.h>

#include "Core/Task.h"
#include "Core/User.h"
#include "Core/Redmine/WindowRetriever.h"
#include "Core/Redmine/RedmineModel.h"

namespace Redmine {

/** @brief IssuesRetriever extends WindowRetriever to receive Redmine issues JSON data.
 *  Issues are downloaded from Redmine in chunks (windows). IssueRetriever is a Collection that
 *  retrieves the first window, and then generates job elements to retrieve the remaining issues.
 */
class IssuesRetriever : public WindowRetriever
{
public:
    explicit IssuesRetriever(Model* model, Configuration* config);

protected:
    void run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread* thread) override;
    QUrlQuery setupQuery() override;

    virtual void setupSubwindowQueries();

private:
    Model* model_;
};

}

#endif // ISSUESRETRIEVER_H
