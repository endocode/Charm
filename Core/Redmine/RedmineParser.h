#ifndef REDMINEPARSER_H
#define REDMINEPARSER_H

#include <Core/Task.h>
#include <QtCore>

namespace Redmine {

namespace Parser {

Task parseProject(const QJsonObject& project);
Task parseIssue(const QJsonObject& issue);

TaskList buildTaskList(QJsonArray projects, QJsonArray issues);

}

}

#endif // REDMINEPARSER_H
