#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>

#include "ProjectsRetriever.h"
#include "RedmineParser.h"

namespace Redmine {

ProjectsRetriever::ProjectsRetriever(Configuration* config)
    : WindowRetriever(config)
{
    setPath("/projects.json");
}

int ProjectsRetriever::count()
{
    return projects_.count();
}

TaskList ProjectsRetriever::projects() const
{
    return projects_;
}

void ProjectsRetriever::run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread *thread)
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
    const QJsonArray projectsArray = json["projects"].toArray();
    if (projectsArray.count() > limit()) {
        setSuccess(false);
        return;
    }
    std::transform(projectsArray.begin(), projectsArray.end(), std::back_inserter(projects_),
                   [this](const QJsonValue& v) { return Parser::parseProject(v.toObject()); } );
}



}
