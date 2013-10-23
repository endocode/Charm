#ifndef REDMINEPARSER_H
#define REDMINEPARSER_H

#include <QtCore>

#include <Core/Task.h>
#include <Core/User.h>

namespace Redmine {

namespace Parser {

Task parseProject(const QJsonObject& project);
Task parseIssue(const QJsonObject& issue);
User parseUser(const QJsonObject& user);

TaskList buildTaskList(QJsonArray projects, QJsonArray issues);

}

}

#endif // REDMINEPARSER_H
