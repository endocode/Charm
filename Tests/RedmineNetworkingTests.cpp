#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <threadweaver/ThreadWeaver.h>
#include <threadweaver/debuggingaids.h>

#include <Core/Redmine/RedmineConfiguration.h>
#include <Core/Redmine/RedmineRetriever.h>
#include <Core/CharmExceptions.h>
#include <Core/Redmine/IssuesRetriever.h>
#include <Core/Redmine/ProjectsRetriever.h>
#include <Core/Redmine/StatusRetriever.h>
#include <Core/Redmine/ModelDownloader.h>
#include <Core/Redmine/RedmineConnector.h>
#include <Core/Redmine/RedmineModel.h>

#include <Core/Logging/Facility.h>
#include <Core/Logging/Macros.h>

class RedmineNetworkingTests : public QObject
{
    Q_OBJECT
    
public:
    RedmineNetworkingTests();
    Redmine::Configuration* configuration();

private Q_SLOTS:
    void testRetriever();
    void testIssuesRetriever();
    void testStatusRetriever();
    void testUsersRetriever();
    void testProjectsRetriever();
    void testModelDownload();
    void testAbortConnections();
    void testRedmineConnector();

private:
    Redmine::Configuration configuration_;    
    Logging::Facility log;
};

RedmineNetworkingTests::RedmineNetworkingTests()
{
//    ThreadWeaver::setDebugLevel(true, 2);
    QCoreApplication::instance()->setOrganizationName("Endocode_AG");
    QCoreApplication::instance()->setOrganizationDomain("endocode.com");
    QCoreApplication::instance()->setApplicationName("RedmineNetworkingTests");
    log.setupLogging(Logging::Facility::LogLevel_Trace);
    TRACE("Executing RedmineNetworkingTests...");

    const QString server = qgetenv("CHARM_REDMINE_SERVER");
    if (server.isEmpty()) {
        throw CharmException("Environment variable CHARM_REDMINE_SERVER is not set!");
    }
    INFO(tr("Charm Redmine server: %1").arg(server));
    const QString apiKey = qgetenv("CHARM_REDMINE_APIKEY");
    if (apiKey.isEmpty()) {
        throw CharmException("Environment variable CHARM_REDMINE_APIKEY is not set!");
    }
    INFO(tr("Charm Redmine API key: %1").arg(apiKey));
    configuration_.setServer(QUrl(server));
    configuration_.setApiKey(apiKey);
    qRegisterMetaType<TaskList>("TaskList");
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
    using namespace Redmine;
    using namespace ThreadWeaver;
    Redmine::Model model;
    Redmine::IssuesRetriever retriever(&model, configuration());
    stream() << retriever;
    Queue::instance()->finish();
    QCOMPARE(retriever.success(), true);
    TRACE(tr("RedmineNetworkingTests::testIssuesRetriever: Tasks retrieved: %1").arg(model.tasks().count()));
    QCOMPARE(model.tasks().count(), 10002);
}

void RedmineNetworkingTests::testStatusRetriever()
{
    Redmine::Model model;
    Redmine::StatusRetriever retriever(&model, configuration());
    retriever.blockingExecute();
    QCOMPARE(retriever.success(), true);
}

void RedmineNetworkingTests::testUsersRetriever()
{
    QFAIL("NI");
}

void RedmineNetworkingTests::testProjectsRetriever()
{
    using namespace Redmine;
    using namespace ThreadWeaver;

    Model model;
    ProjectsRetriever retriever(&model, configuration());
    stream() << retriever;
    Queue::instance()->finish();
    QCOMPARE(retriever.success(), true);
    QCOMPARE(model.tasks().count(), 112);
    TRACE(tr("RedmineNetworkingTests::testProjectsRetriever: Projects retrieved: %1").arg(model.tasks().count()));
}

void RedmineNetworkingTests::testModelDownload()
{
    using namespace Redmine;

    QScopedPointer<ModelDownloader> downloader(new ModelDownloader(configuration()));
    QEventLoop loop;
    QSignalSpy errorSpy(downloader.data(), SIGNAL(error()));
    connect(downloader.data(), SIGNAL(completed()), &loop, SLOT(quit()));
    connect(downloader.data(), SIGNAL(error()), &loop, SLOT(quit()));
    downloader->downloadModelData();
    loop.exec();
    QVERIFY(errorSpy.count() == 0);
    ThreadWeaver::Queue::instance()->finish();
    TRACE(tr("RedmineNetworkingTests::testTaskListProvider: Retrieved %1 tasks").arg(downloader->model()->tasks().count()));
}

void RedmineNetworkingTests::testAbortConnections()
{
    QFAIL("NI");
//    Redmine::Model model;
//    Redmine::TaskListProvider provider(configuration());
//    QEventLoop loop;
//    QSignalSpy errorSpy(&provider, SIGNAL(error()));
//    connect(&provider, SIGNAL(completed()), &loop, SLOT(quit()));
//    connect(&provider, SIGNAL(error()), &loop, SLOT(quit()));
//    provider.downloadModelData(&model);

//    QTimer timer;
//    timer.singleShot(500, &provider, SLOT(abortCurrentOperations()));
//    loop.exec();

//    QTest::qWait(2000);
//    qDebug() << errorSpy.count();
//    QTest::qWait(2000);
//    qDebug() << errorSpy.count();
//    QTest::qWait(2000);
}


void RedmineNetworkingTests::testRedmineConnector()
{
    using namespace Redmine;
    using namespace ThreadWeaver;

    Redmine::Connector connector;
//    ThreadWeaver::QJobPointer updater = ThreadWeaver::enqueue_qjob(ModelUpdater, &model, configuration());
    // Sequence:
    // * Collection (CurrentUserRetriever, StatusRetriever)
    // * Collection (IssuesRetriever, ProjectsRetriever, UsersRetriever)
    // * TreeBuilder
    // ... further enqueue: Gravatar downloads

    // wait for updater to complete (using loop)
    // ...
    // verify results:
    // ...
//    QEventLoop loop;
//    connect(&connector, SIGNAL(updatedTaskList(TaskList)), &loop, SLOT(quit()));
//    connect(&connector, SIGNAL(connectorError(QString)), &loop, SLOT(quit()));
//    loop.exec();
}

QTEST_MAIN(RedmineNetworkingTests)

#include "RedmineNetworkingTests.moc"
