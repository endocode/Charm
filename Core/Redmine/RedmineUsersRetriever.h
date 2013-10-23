#ifndef REDMINEUSERSRETRIEVER_H
#define REDMINEUSERSRETRIEVER_H

#include "WindowRetriever.h"

namespace Redmine {

class UsersRetriever : public WindowRetriever
{
public:
    UsersRetriever(Configuration* config = 0);

protected:
    void run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread* thread) override;
};

}

#endif // REDMINEUSERSRETRIEVER_H
