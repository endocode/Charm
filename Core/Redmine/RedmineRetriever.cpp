#include "RedmineRetriever.h"

#include <QtDebug>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>

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

void Retriever::run(ThreadWeaver::JobPointer, ThreadWeaver::Thread*)
{
    QNetworkAccessManager manager;
    QEventLoop loop;
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    QUrl url = configuration_->server();
    url.setPath(url.path() + path());
    QUrlQuery query;
    query.addQueryItem("key", configuration_->apiKey());
    url.setQuery(query);
    auto reply = manager.get(QNetworkRequest(url));
    loop.exec();
    qDebug() << "Retriever::run: fetching URL" << url.toString();
    if (reply->error() == QNetworkReply::NoError) {
        success_ = true;
        data_ = reply->readAll();
        qDebug() << "Retriever::run: success, received" << data_.count() << "bytes.";
    } else {
        success_ = false;
        qDebug() << "Retriever::run: error" << qPrintable(reply->errorString());
    }
}

}
