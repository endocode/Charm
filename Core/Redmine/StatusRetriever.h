#ifndef STATUSRETRIEVER_H
#define STATUSRETRIEVER_H

#include <QVector>

#include <Core/Redmine/IssueStatus.h>
#include <Core/Redmine/RedmineConfiguration.h>
#include <Core/Redmine/RedmineRetriever.h>

namespace Redmine {

class Model;

/** @brief Retrieves issues statuses from  Redmine. */
class StatusRetriever : public Retriever
{
public:
    StatusRetriever(Model* model, Configuration* config = 0);
    QVector<Redmine::Status> statuses() const;
    Redmine::Status status(Redmine::Status::Id id) const;

protected:
    void run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread* thread) override;

private:
    QVector<Redmine::Status> statuses_;
    Model* model_;
};

}

#endif // STATUSRETRIEVER_H
