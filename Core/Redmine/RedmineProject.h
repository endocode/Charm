#ifndef REDMINEPROJECT_H
#define REDMINEPROJECT_H

//#include <set>

#include <Core/Task.h>
#include <Core/Redmine/RedmineIssue.h>
#include <Core/Redmine/RedmineProject.h>

namespace Redmine {

//FIXME flat model, pointer tree or tree model?
class Project : public Task
{
public:
    Project();
//    const std::set<Project>& projects() const;
//    const std::set<Issue>& issues() const;

private:
//    std::set<Project> subProjects_;
//    std::set<Issue> issues_;
};

}

#endif // REDMINEPROJECT_H
