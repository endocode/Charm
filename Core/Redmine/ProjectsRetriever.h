#ifndef PROJECTSRETRIEVER_H
#define PROJECTSRETRIEVER_H

#include <threadweaver/Job.h>
#include <Core/Redmine/RedmineConfiguration.h>
#include <Core/Redmine/RedmineModel.h>

#include "Core/Task.h"
#include "Core/Redmine/WindowRetriever.h"

namespace Redmine {

/** @brief ProjectsRetriever extends WindowRetriever to receive Redmine projects JSON data. */
class ProjectsRetriever : public WindowRetriever
{
public:
    explicit ProjectsRetriever(Model* model, Configuration* config);

protected:
    void run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread* thread) override;
    virtual void setupSubwindowQueries();

private:
    Model* model_;
};

};

#endif // PROJECTSRETRIEVER_H
