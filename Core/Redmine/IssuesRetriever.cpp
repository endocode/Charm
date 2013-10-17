#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>

#include "IssuesRetriever.h"
#include "RedmineParser.h"

namespace Redmine {

IssuesRetriever::IssuesRetriever(Configuration* config)
    : WindowRetriever(config)
{
    setPath("/issues.json");
}

int IssuesRetriever::count() const
{
    return issues_.count();
}

TaskList IssuesRetriever::issues() const
{
    return issues_;
}

void IssuesRetriever::run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread *thread)
{
    WindowRetriever::run(job, thread);
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
    const QJsonArray issuesArray = json["issues"].toArray();
    if (issuesArray.count() > limit()) {
        setSuccess(false);
        return;
    }
    std::transform(issuesArray.begin(), issuesArray.end(), std::back_inserter(issues_),
                   [this](const QJsonValue& v) { return Parser::parseIssue(v.toObject()); } );
}

}
