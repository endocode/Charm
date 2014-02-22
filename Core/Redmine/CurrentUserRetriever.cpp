#include <QtCore>

#include <Core/Logging/Macros.h>

#include "RedmineModel.h"
#include "RedmineParser.h"
#include "CurrentUserRetriever.h"

namespace Redmine {

CurrentUserRetriever::CurrentUserRetriever(Model *model, Configuration *config)
    : Retriever(config)
    , model_(model)
{
    Q_ASSERT(model);
    setPath("/users/current.json");
}

User CurrentUserRetriever::currentUser() const
{
    return current_;
}

void CurrentUserRetriever::run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread *thread)
{
    TRACE(QObject::tr("CurrentUserRetriever::run: retrieving current user."));
    Retriever::run(job, thread);
    if (!success()) {
        setSuccess(false);
        return;
    }
    QJsonParseError jsonParseError;
    const QJsonDocument jsonDoc =QJsonDocument::fromJson(data(), &jsonParseError);
    if (jsonParseError.error != QJsonParseError::NoError) {
        setSuccess(false);
        DEBUG(QObject::tr("CurrentUserRetriever::run: JSON document format error"));
        return;
    }

    const QJsonObject json = jsonDoc.object();
    const QJsonObject user = json["user"].toObject();
    model_->setCurrentUser(Parser::parseUser(user));
    TRACE(QObject::tr("CurrentUserRetriever::run: done (current user: %1), model updated.").arg(model_->currentUser().name()));
}


}
