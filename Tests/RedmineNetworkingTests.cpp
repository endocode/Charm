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
    configuration.setServer(QUrl("https://endocode.com/chili"));
    configuration.setApiKey("fb835c8b6286ccb5e712c2ca6e51322ce8299a6c");

    Redmine::Retriever retriever(&configuration);
    retriever.setUrl(QUrl("https://endocode.com/chili/issues.json"));
    retriever.blockingExecute();

    QCOMPARE(retriever.success(), true);
}

QTEST_MAIN(RedmineNetworkingTests)

#include "RedmineNetworkingTests.moc"
