#ifndef REDMINERETRIEVER_H
#define REDMINERETRIEVER_H

#include <QMutex>

#include <ThreadWeaver/Collection>
#include <Core/Redmine/RedmineConfiguration.h>

class QNetworkReply;

namespace Redmine {

/** @brief Retriever implements accessing the Redmine API based on the given configuration. */
class Retriever : public ThreadWeaver::Collection
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

    Configuration* configuration() const;

private:
    Configuration* configuration_;
    QString path_;
    QByteArray data_;
    bool success_;
    QNetworkReply* reply_;
    QMutex mutex_;
};

/** @brief Return the numeric id of the thread, or zero if th is zero. */
int thread_id(ThreadWeaver::Thread* th);

}

#endif // REDMINERETRIEVER_H
