#include "RedmineConfiguration.h"

namespace Redmine {

Configuration::Configuration(QObject *parent)
    : QObject(parent)
    , network_(2 )
{
}

QUrl Configuration::server() const
{
    return server_;
}

void Configuration::setServer(const QUrl& url)
{
    server_ = url;
}

QString Configuration::apiKey() const
{
    return apiKey_;
}

void Configuration::setApiKey(const QString &key)
{
    apiKey_ = key;
}

ThreadWeaver::ResourceRestrictionPolicy *Configuration::network()
{
    return &network_;
}

}
