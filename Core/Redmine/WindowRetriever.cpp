#include <QtCore>

#include <Core/Logging/Macros.h>

#include "WindowRetriever.h"

namespace Redmine {

WindowRetriever::WindowRetriever(Configuration *config)
    : Retriever(config)
    , offset_()
    , limit_()
    , total_()
    , requestOffset_()
    , requestLimit_()
{
}

int WindowRetriever::offset() const
{
    return offset_;
}

int WindowRetriever::limit() const
{
    return limit_;
}

int WindowRetriever::total() const
{
    return total_;
}

QUrlQuery WindowRetriever::setupQuery()
{
    QUrlQuery query = Retriever::setupQuery();
    if (requestLimit_ != 0) {
        query.addQueryItem("offset", QString::number(requestOffset_));
        query.addQueryItem("limit", QString::number(requestLimit_));
    }
    return query;
}

void WindowRetriever::run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread *thread)
{
    Retriever::run(job, thread);
    if (!success()) {
        return;
    }
    QJsonParseError jsonParseError;
    const QJsonDocument jsonDoc =QJsonDocument::fromJson(data(), &jsonParseError);
    if (jsonParseError.error != QJsonParseError::NoError) {
        setSuccess(false);
        return;
    }
    const QJsonObject json = jsonDoc.object();
    const QVariantMap documentData = json.toVariantMap();
    if (documentData.contains("limit")) {
        limit_ = documentData["limit"].toInt();
        total_ = documentData["total_count"].toInt();
        offset_ = documentData["offset"].toInt();
        DEBUG(QObject::tr("WindowRetriever::run: success - offset %1, limit %2, total %3")
                .arg(offset_).arg(limit_).arg(total_));
    } else {
        ERROR(QObject::tr("WindowRetriever::run: error parsing data"));
        setSuccess(false);
        return;
    }
}

void Redmine::WindowRetriever::setWindow(int offset, int limit)
{
    requestOffset_ = offset;
    requestLimit_ = limit;
}

}

