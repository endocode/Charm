#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>

#include "ProjectsRetriever.h"
#include "RedmineParser.h"

namespace Redmine {

class ProjectsSubwindowRetriever : public ProjectsRetriever {
public:
    explicit ProjectsSubwindowRetriever(Model* model, Configuration* config) : ProjectsRetriever(model, config) {}
protected:
    void setupSubwindowQueries() override {}
};

ProjectsRetriever::ProjectsRetriever(Model *model, Configuration* config)
    : WindowRetriever(config)
    , model_(model)
{
    setPath("/projects.json");
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
    TaskList projects_;
    std::transform(projectsArray.begin(), projectsArray.end(), std::back_inserter(projects_),
                   [this](const QJsonValue& v) { return Parser::parseProject(v.toObject()); } );
    model_->appendTasks(projects_);
    setupSubwindowQueries();
}

void ProjectsRetriever::setupSubwindowQueries()
{
    // create further requests for the remaining issues:
    if (limit() < total()) {
        for(int current = offset() + limit(); current < total(); current += limit() ) {
            auto chunk = new ProjectsSubwindowRetriever(model_, configuration());
            chunk->setWindow(current, limit());
            *this << chunk;
        }
    }

}

}
