#ifndef REDMINEUSERSRETRIEVER_H
#define REDMINEUSERSRETRIEVER_H

#include "WindowRetriever.h"

namespace Redmine {

class Model;

class UsersRetriever : public WindowRetriever
{
public:
    UsersRetriever(Model* model, Configuration* config = 0);

protected:
    void run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread* thread) override;

private:
    Model* model_;
};

}

#endif // REDMINEUSERSRETRIEVER_H
