#ifndef STATUSRETRIEVER_H
#define STATUSRETRIEVER_H

#include <QVector>

#include <Core/Redmine/RedmineConfiguration.h>
#include <Core/Redmine/RedmineRetriever.h>
#include <Core/Redmine/Status.h>

namespace Redmine {

/** @brief Retrieves issues statuses from  Redmine. */
class StatusRetriever : public Retriever
{
public:
    StatusRetriever(Configuration* config = 0);
    QVector<Status> statuses() const;

protected:
    void run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread* thread) override;

private:
    QVector<Status> statuses_;
};

}

#endif // STATUSRETRIEVER_H
