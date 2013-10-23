#include <QtCore>

#include "RedmineParser.h"
#include "CurrentUserRetriever.h"

namespace Redmine {

CurrentUserRetriever::CurrentUserRetriever(Configuration *config)
    : Retriever(config)
{
    setPath("/users/current.json");
}

User CurrentUserRetriever::currentUser() const
{
    return current_;
}

void CurrentUserRetriever::run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread *thread)
{
    Retriever::run(job, thread);
    if (!success()) {
        setSuccess(false);
        return;
    }
    QJsonParseError jsonParseError;
    const QJsonDocument jsonDoc =QJsonDocument::fromJson(data(), &jsonParseError);
    if (jsonParseError.error != QJsonParseError::NoError) {
        setSuccess(false);
        return;
    }

    const QJsonObject json = jsonDoc.object();
    const QJsonObject user = json["user"].toObject();
    current_ = Parser::parseUser(user);
}


}
