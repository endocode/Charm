#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>

#include <Core/Logging/Macros.h>

#include "RedmineUsersRetriever.h"
#include "RedmineParser.h"
#include "RedmineModel.h"

namespace Redmine {

UsersRetriever::UsersRetriever(Model *model, Configuration *config)
    : WindowRetriever(config)
    , model_(model)
{
    Q_ASSERT(model);
    setPath("/users.json");
}

void UsersRetriever::run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread *thread)
{
    TRACE(QObject::tr("UsersRetriever::run: retrieving all users."));
    WindowRetriever::run(job, thread);
    if (!success()) {
        setSuccess(false);
        return;
    }
    QJsonParseError jsonParseError;
    const QJsonDocument jsonDoc =QJsonDocument::fromJson(data(), &jsonParseError);
    if (jsonParseError.error != QJsonParseError::NoError) {
        DEBUG(QObject::tr("UsersRetriever::run: JSON document format error"));
        setSuccess(false);
        return;
    }

    const QJsonObject json = jsonDoc.object();
    const QJsonArray usersArray = json["users"].toArray();
    if (usersArray.count() > limit()) {
        setSuccess(false);
        return;
    }
    QVector<User> users;
    std::transform(usersArray.begin(), usersArray.end(), std::back_inserter(users),
                   [this](const QJsonValue& v) { return Parser::parseUser(v.toObject()); } );
    model_->setUsers(users);
    TRACE(QObject::tr("UsersRetriever::run: done (%1 users), model updated.").arg(users.size()));
}

}

