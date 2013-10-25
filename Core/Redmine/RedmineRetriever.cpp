#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>

#include <Core/Logging/Macros.h>

#include "RedmineRetriever.h"

namespace Redmine {

Retriever::Retriever(Configuration *config)
    : configuration_ (config)
    , success_(false)
{
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
    query.addQueryItem("key", configuration_->apiKey());
    return query;
}

void Retriever::run(ThreadWeaver::JobPointer, ThreadWeaver::Thread*)
{
    QNetworkAccessManager manager;
    QEventLoop loop;
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    QUrl url = configuration_->server();
    url.setPath(url.path() + path());
    url.setQuery(setupQuery());
    auto reply = manager.get(QNetworkRequest(url));
    loop.exec();
    DEBUG(QObject::tr("Retriever::run: fetching URL %1").arg(url.toString()));
    if (reply->error() == QNetworkReply::NoError) {
        setSuccess(true);
        data_ = reply->readAll();
        DEBUG(QObject::tr("Retriever::run: success, received %1 bytes").arg(data_.count()));
    } else {
        setSuccess(false);
        DEBUG(QObject::tr("Retriever::run: error: %1").arg(reply->errorString()));
    }
}

}
