#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "RedmineUsersRetriever.h"

namespace Redmine {

UsersRetriever::UsersRetriever(Configuration *config)
    : WindowRetriever(config)
{
    setPath("/users.json");
}

void UsersRetriever::run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread *thread)
{
    WindowRetriever::run(job, thread);
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
    const QJsonArray projectsArray = json["users"].toArray();
    if (projectsArray.count() > limit()) {
        setSuccess(false);
        return;
    }
    //    std::transform(projectsArray.begin(), projectsArray.end(), std::back_inserter(projects_),
    //                   [this](const QJsonValue& v) { return Parser::parseProject(v.toObject()); } );
}

}

