#include "StatusRetriever.h"
#include "RedmineParser.h"

#include <Core/Logging/Macros.h>
#include "RedmineModel.h"

namespace Redmine {

StatusRetriever::StatusRetriever(Model* model, Configuration* configuration)
    : Retriever(configuration)
    , model_(model)
{
    setPath("/issue_statuses.json");
}

QVector<Status> StatusRetriever::statuses() const
{
    return statuses_;
}

Status StatusRetriever::status(Status::Id id) const
{
    //FIXME use a status set that compares by Id
    Q_FOREACH(const Status& status, statuses_) {
        if (status.id() == id) {
            return status;
        }
    }
    return Status();
}

void StatusRetriever::run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread *thread)
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
    Q_ASSERT(statuses_.isEmpty());
    QJsonArray statusesArray = jsonDoc.object()["issue_statuses"].toArray();
    std::transform(statusesArray.begin(), statusesArray.end(), std::back_inserter(statuses_),
                   [](const QJsonValue& element) { return Parser::parseStatus(element.toObject()); } );
    if (statuses_.count() == statusesArray.count()) {
        DEBUG(QObject::tr("StatusRetriever::run: success, %1 statuses downloaded").arg(statuses_.count()));
        model_->setIssueStatuses(statuses_);
    } else {
        DEBUG(QObject::tr("StatusRetriever::run: error downloading statuses"));
    }
}

}
