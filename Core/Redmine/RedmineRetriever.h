#ifndef REDMINERETRIEVER_H
#define REDMINERETRIEVER_H

#include <threadweaver/Job.h>
#include <Redmine/RedmineConfiguration.h>

namespace Redmine {

/** @brief Retriever implements accessing the Redmine API based on the given configuration. */
class Retriever : public ThreadWeaver::Job
{
public:
    Retriever(Configuration* config = 0);
    QString path() const;
    void setPath(const QString& path);
    QByteArray data() const;
    bool success() const;

protected:
    void setSuccess(bool success);
    virtual QUrlQuery setupQuery();
    void run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread* thread);

private:
    Configuration* configuration_;
    QString path_;
    QByteArray data_;
    bool success_;
};

}

#endif // REDMINERETRIEVER_H
