#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include <Core/Redmine/RedmineConfiguration.h>
#include <Core/Redmine/RedmineRetriever.h>
#include <Core/CharmExceptions.h>
#include <Core/Redmine/IssuesRetriever.h>
#include <Core/Redmine/ProjectsRetriever.h>

class RedmineNetworkingTests : public QObject
{
    Q_OBJECT
    
public:
    RedmineNetworkingTests();
    Redmine::Configuration* configuration();

private Q_SLOTS:
    void testRetriever();
    void testIssuesRetriever();
    void testProjectsRetriever();

private:
    Redmine::Configuration configuration_;
};

RedmineNetworkingTests::RedmineNetworkingTests()
{
    const QString server = qgetenv("CHARM_REDMINE_SERVER");
    if (server.isEmpty()) {
        throw CharmException("Environment variable CHARM_REDMINE_SERVER is not set!");
    }
    const QString apiKey = qgetenv("CHARM_REDMINE_APIKEY");
    if (apiKey.isEmpty()) {
        throw CharmException("Environment variable CHARM_REDMINE_APIKEY is not set!");
    }
    configuration_.setServer(QUrl(server));
    configuration_.setApiKey(apiKey);
}

Redmine::Configuration *RedmineNetworkingTests::configuration()
{
    return &configuration_;
}

void RedmineNetworkingTests::testRetriever()
{
    Redmine::Retriever retriever(configuration());
    retriever.setPath("/issues.json");
    retriever.blockingExecute();

    QCOMPARE(retriever.success(), true);
}

void RedmineNetworkingTests::testIssuesRetriever()
{
    Redmine::IssuesRetriever retriever(configuration());
    retriever.blockingExecute();
    QCOMPARE(retriever.success(), true);
    qDebug() << "Results: offset" << retriever.offset() << "- limit"
             << retriever.limit() << "- total" << retriever.total();

    TaskList tasks;
    tasks << retriever.issues();
    if (retriever.limit() < retriever.total()) {
        for(int current = retriever.offset() + retriever.limit(); current < retriever.total(); current += retriever.limit() ) {
            Redmine::IssuesRetriever chunk(configuration());
            chunk.setWindow(current, retriever.limit());
            chunk.blockingExecute();
            QCOMPARE(chunk.success(), true);
            tasks << chunk.issues();
        }
    }
    qDebug() << "Retrieved" << tasks.count() << "issues.";
}

void RedmineNetworkingTests::testProjectsRetriever()
{
    Redmine::ProjectsRetriever retriever(configuration());
    retriever.setWindow(0, 4);
    retriever.blockingExecute();
    QCOMPARE(retriever.success(), true);
    qDebug() << "Results: offset" << retriever.offset() << "- limit"
             << retriever.limit() << "- total" << retriever.total();

    TaskList tasks;
    tasks << retriever.projects();
    if (retriever.limit() < retriever.total()) {
        for(int current = retriever.offset() + retriever.limit(); current < retriever.total(); current += retriever.limit() ) {
            Redmine::ProjectsRetriever chunk(configuration());
            chunk.setWindow(current, retriever.limit());
            chunk.blockingExecute();
            QCOMPARE(chunk.success(), true);
            tasks << chunk.projects();
        }
    }
    qDebug() << "Retrieved" << tasks.count() << "projects.";
}

QTEST_MAIN(RedmineNetworkingTests)

#include "RedmineNetworkingTests.moc"
