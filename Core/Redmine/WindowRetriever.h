#ifndef WINDOWRETRIEVER_H
#define WINDOWRETRIEVER_H

#include <QUrlQuery>

#include <Core/Redmine/RedmineRetriever.h>

namespace Redmine {

class WindowRetriever : public Retriever
{
public:
    WindowRetriever(Configuration* config = 0);
    void setWindow(int offset, int limit);

    int offset() const;
    int limit() const;
    int total() const;

protected:
    QUrlQuery setupQuery() override;
    void run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread* thread) override;


private:
    int offset_, limit_, total_, requestOffset_, requestLimit_;
};

}

#endif // WINDOWRETRIEVER_H
