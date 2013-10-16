#include "RedmineRetriever.h"

namespace Redmine {

Retriever::Retriever(Configuration *config)
    : configuration_ (config)
    , success_(false)
{
}

QUrl Retriever::url() const
{
    return url_;
}

void Retriever::setUrl(QUrl url)
{
    url_ = url;
}

QByteArray Retriever::data() const
{
    return data_;
}

bool Retriever::success() const
{
    return success_;
}

void Retriever::run(ThreadWeaver::JobPointer job, ThreadWeaver::Thread *thread)
{

}

}
