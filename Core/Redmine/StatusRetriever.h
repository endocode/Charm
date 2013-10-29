#ifndef STATUSRETRIEVER_H
#define STATUSRETRIEVER_H

#include <QVector>

#include <Core/Redmine/RedmineConfiguration.h>
#include <Core/Redmine/RedmineRetriever.h>
#include <Core/Redmine/IssueStatus.h>

namespace Redmine {

class Model;

/** @brief Retrieves issues statuses from  Redmine. */
class StatusRetriever : public Retriever
{
public:
    StatusRetriever(Model* model, Configuration* config = 0);
    QVector<Status> statuses() const;
    Status status(Status::Id id) const;

protected:
    void run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread* thread) override;

private:
    QVector<Status> statuses_;
    Model* model_;
};

}

#endif // STATUSRETRIEVER_H
