#ifndef REDMINEPARSER_H
#define REDMINEPARSER_H

#include <QtCore>

#include <Core/Task.h>
#include <Core/User.h>
#include <Core/Redmine/Status.h>

namespace Redmine {

namespace Parser {

Task parseProject(const QJsonObject& project);
Task parseIssue(const QJsonObject& issue, const User& me);
User parseUser(const QJsonObject& user);
Status parseStatus(const QJsonObject& status);

//TaskList buildTaskList(QJsonArray projects, QJsonArray issues);

}

}

#endif // REDMINEPARSER_H
