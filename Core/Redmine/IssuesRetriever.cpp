#include "IssuesRetriever.h"
#include "RedmineConnector.h"
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>


namespace Redmine {

IssuesRetriever::IssuesRetriever(Configuration* config)
    : Retriever(config)
    , offset_()
    , limit_()
    , total_()
    , requestOffset_()
    , requestLimit_()
{
    setPath("/issues.json");
}

void IssuesRetriever::setWindow(int offset, int limit)
{
    requestOffset_ = offset;
    requestLimit_ = limit;
}

QUrlQuery IssuesRetriever::setupQuery()
{
    QUrlQuery query = Retriever::setupQuery();
    if (requestLimit_ != 0) {
        query.addQueryItem("offset", QString::number(requestOffset_));
        query.addQueryItem("limit", QString::number(requestLimit_));
    }
    return query;
}

void IssuesRetriever::run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread *thread)
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
    } else {
        setSuccess(false);
        return;
    }
    const QJsonArray issuesArray = json["issues"].toArray();
    if (issuesArray.count() > limit_) {
        setSuccess(false);
        return;
    }
    std::transform(issuesArray.begin(), issuesArray.end(), std::back_inserter(issues_),
                   [this](const QJsonValue& v) { return Connector::parseIssue(v.toObject()); } );
}

}
