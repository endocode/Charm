#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>

#include <threadweaver/Thread.h>

#include <Core/Logging/Macros.h>

#include "RedmineRetriever.h"

namespace Redmine {

Retriever::Retriever(Configuration *config)
    : configuration_ (config)
    , success_(false)
    , reply_(0)
{
//    assignQueuePolicy(configuration_->network());
}

QString Retriever::path() const
{
    return path_;
}

void Retriever::setPath(const QString &path)
{
    path_ = path;
}

QByteArray Retriever::data() const
{
    return data_;
}

bool Retriever::success() const
{
    return success_;
}

void Retriever::setSuccess(bool success)
{
    success_ = success;
}

QUrlQuery Retriever::setupQuery()
{
    QUrlQuery query;
    return query;
}

void Retriever::run(ThreadWeaver::JobPointer, ThreadWeaver::Thread* th)
{
    Q_ASSERT(reply_ == 0);
    TRACE(QObject::tr("Retriever::run[%1]: initiating API query.").arg(thread_id(th)));
    QNetworkAccessManager manager;
    QEventLoop loop;
//    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    QUrl url = configuration_->server();
    url.setPath(url.path() + path());
    url.setQuery(setupQuery());
    QNetworkRequest request(url);
    request.setRawHeader(QByteArray("X-Redmine-API-Key"), configuration_->apiKey().toLocal8Bit());
    {
        QMutexLocker l(&mutex_);
        reply_ = manager.get(request);
    }
    QObject::connect(reply_, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply_, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();
    DEBUG(QObject::tr("Retriever::run[%1]: fetching URL %2").arg(thread_id(th)).arg(url.toString()));
    if (reply_->error() == QNetworkReply::NoError) {
        setSuccess(true);
        data_ = reply_->readAll();
        DEBUG(QObject::tr("Retriever::run[%1]: success, received %2 bytes").arg(thread_id(th)).arg(data_.count()));
    } else {
        setSuccess(false);
        DEBUG(QObject::tr("Retriever::run[%1]: error: %2").arg(thread_id(th)).arg(reply_->errorString()));
    }
    {
        QMutexLocker l(&mutex_);
        delete reply_;
        reply_ = 0;
    }
}

void Retriever::requestAbort()
{
    QMutexLocker l(&mutex_);
    if (reply_ != 0) {
        TRACE(QObject::tr("Retriever::requestAbort: aborting current network  operations."));
        reply_->abort();
    } else {
        TRACE(QObject::tr("Retriever::requestAbort: no current network  operations."));
    }
}

Configuration *Retriever::configuration() const
{
    return configuration_;
}

int thread_id(ThreadWeaver::Thread *th)
{
    if (th) {
        return th->id();
    } else {
        return 0;
    }
}

}
