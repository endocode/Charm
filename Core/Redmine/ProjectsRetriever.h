#ifndef PROJECTSRETRIEVER_H
#define PROJECTSRETRIEVER_H

#include <threadweaver/Job.h>
#include <Core/Redmine/RedmineConfiguration.h>

#include "Core/Task.h"
#include "Core/Redmine/WindowRetriever.h"

namespace Redmine {

/** @brief ProjectsRetriever extends WindowRetriever to receive Redmine projects JSON data. */
class ProjectsRetriever : public WindowRetriever
{
public:
    ProjectsRetriever(Configuration* config = 0);
    int count();
    TaskList projects() const;

protected:
    void run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread* thread) override;

private:
    TaskList projects_;
};

};

#endif // PROJECTSRETRIEVER_H
