#ifndef REDMINERETRIEVER_H
#define REDMINERETRIEVER_H

#include <QMutex>

#include <threadweaver/Job.h>
#include <Redmine/RedmineConfiguration.h>

class QNetworkReply;

namespace Redmine {

//FIXME make model_ a member of Retriever?
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
    void run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread* thread) override;
    void requestAbort() override;

private:
    Configuration* configuration_;
    QString path_;
    QByteArray data_;
    bool success_;
    QNetworkReply* reply_;
    QMutex mutex_;
};

}

#endif // REDMINERETRIEVER_H
