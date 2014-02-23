#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>

#include <ThreadWeaver/Thread>

#include <Core/Logging/Macros.h>

#include "RedmineModel.h"
#include "IssuesRetriever.h"
#include "RedmineParser.h"

namespace Redmine {

class IssueSubwindowRetriever : public IssuesRetriever {
public:
    explicit IssueSubwindowRetriever(Model* model, Configuration* config) : IssuesRetriever(model, config) {}
protected:
    void setupSubwindowQueries() override {}
};


IssuesRetriever::IssuesRetriever(Model *model, Configuration* config)
    : WindowRetriever(config)
    , model_(model)
{
    setPath("/issues.json");
}

void IssuesRetriever::run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread *thread)
{
    Q_ASSERT(model_->currentUser().isValid());
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
    TaskList issues;
    std::transform(issuesArray.begin(), issuesArray.end(), std::back_inserter(issues),
                   [this](const QJsonValue& v) { return Parser::parseIssue(v.toObject(), model_->currentUser()); } );
    model_->appendTasks(issues);
    setupSubwindowQueries();
}

QUrlQuery IssuesRetriever::setupQuery()
{
    QUrlQuery query = WindowRetriever::setupQuery();
    query.addQueryItem("status_id", "*");
    query.addQueryItem("include", "journals");
    return query;
}

void IssuesRetriever::setupSubwindowQueries()
{
    // create further requests for the remaining issues:
    if (limit() < total()) {
        for(int current = offset() + limit(); current < total(); current += limit() ) {
            auto chunk = new IssueSubwindowRetriever(model_, configuration());
            chunk->setWindow(current, limit());
            // DEBUG(QObject::tr("Adding job for window [%1, %2].").arg(current).arg(limit()));
            *this << chunk;
        }
    }
}

}
