#ifndef REDMINECONFIGURATION_H
#define REDMINECONFIGURATION_H

#include <QObject>
#include <QUrl>
#include <QString>

#include <ThreadWeaver/ResourceRestrictionPolicy>

namespace Redmine {

class Configuration : public QObject
{
    Q_OBJECT
public:
    explicit Configuration(QObject *parent = 0);
    QUrl server() const;
    void setServer(const QUrl& url);

    QString apiKey() const;
    void setApiKey(const QString& key);

    ThreadWeaver::ResourceRestrictionPolicy* network();

Q_SIGNALS:
    void changed();

private:
    QUrl server_;
    QString apiKey_;
    ThreadWeaver::ResourceRestrictionPolicy network_;
};

}

#endif // REDMINECONFIGURATION_H
