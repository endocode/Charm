#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include <Core/Redmine/RedmineConfiguration.h>
#include <Core/Redmine/RedmineRetriever.h>

class RedmineNetworkingTests : public QObject
{
    Q_OBJECT
    
public:
    RedmineNetworkingTests();
    
private Q_SLOTS:
    void testRetriever();
};

RedmineNetworkingTests::RedmineNetworkingTests()
{
}

void RedmineNetworkingTests::testRetriever()
{
    Redmine::Configuration configuration;
    const QString server = qgetenv("CHARM_REDMINE_SERVER");
    QVERIFY2(!server.isEmpty(), "Environment variable CHARM_REDMINE_SERVER is not set!");
    const QString apiKey = qgetenv("CHARM_REDMINE_APIKEY");
    QVERIFY2(!apiKey.isEmpty(), "Environment variable CHARM_REDMINE_APIKEY is not set!");
    configuration.setServer(QUrl(server));
    configuration.setApiKey(apiKey);

    Redmine::Retriever retriever(&configuration);
    retriever.setPath("/issues.json");
    retriever.blockingExecute();

    QCOMPARE(retriever.success(), true);
}

QTEST_MAIN(RedmineNetworkingTests)

#include "RedmineNetworkingTests.moc"
