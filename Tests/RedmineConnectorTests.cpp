#include <set>

#include <QString>
#include <QtTest/QtTest>
#include <QCoreApplication>
#include <QJsonDocument>
#include <Core/Redmine/RedmineConnector.h>
#include <Core/Redmine/RedmineParser.h>
#include <Core/Redmine/IssueStatus.h>
#include <Core/Task.h>

#include <Core/Logging/Facility.h>
#include <Core/Logging/Macros.h>

class RedmineConnectorTests : public QObject
{
    Q_OBJECT
public:
    RedmineConnectorTests();
    
private Q_SLOTS:
    void testParseSampleIssue217();
    void testParseSample3Issues();
    void testParseSample18Projects();
    void testParseSampleIssueStatuses();

private:
    QByteArray testData(const QString& filename);
    QJsonDocument testDocument(const QString& filename);
    QJsonObject testObject(const QString& filename);

    Logging::Facility log;
};

RedmineConnectorTests::RedmineConnectorTests()
{
    QCoreApplication::instance()->setOrganizationName("Endocode_AG");
    QCoreApplication::instance()->setOrganizationDomain("endocode.com");
    QCoreApplication::instance()->setApplicationName("RedmineConnectorTests");
    log.setupLogging(Logging::Facility::LogLevel_Trace);
    TRACE("Executing RedmineConnectorTests...");
}

void RedmineConnectorTests::testParseSampleIssue217()
{
    const QJsonObject json = testObject(":/RedmineConnectorTest/Data/RedmineConnector/SampleIssue271.json");
    QVERIFY(!json.isEmpty());
    const QJsonValue issue = json[QString("issue")];
    QVERIFY(!issue.isNull());
    QVERIFY(issue.isObject());
    const Task task = Redmine::Parser::parseIssue(issue.toObject(), User());
    QCOMPARE(task.validFrom().date(), QDate(2013, 8, 20));
}

void RedmineConnectorTests::testParseSample3Issues()
{
    const QJsonObject json = testObject(":/RedmineConnectorTest/Data/RedmineConnector/Sample3Issues.json");
    QVERIFY(!json.isEmpty());
    const QVariantMap documentData = json.toVariantMap();
    QCOMPARE(documentData["limit"].toInt(), 3);
    QCOMPARE(documentData["total_count"].toInt(), 4);
    QCOMPARE(documentData["offset"].toInt(), 0);
    QJsonArray issuesArray = json["issues"].toArray();
    QVERIFY(!issuesArray.isEmpty());
    QCOMPARE(issuesArray.count(), 3);
}

void RedmineConnectorTests::testParseSample18Projects()
{
    const QJsonObject json = testObject(":/RedmineConnectorTest/Data/RedmineConnector/Sample18Projects.json");
    QVERIFY(!json.isEmpty());
    const QVariantMap documentData = json.toVariantMap();
    QCOMPARE(documentData["limit"].toInt(), 25);
    QCOMPARE(documentData["total_count"].toInt(), 18);
    QCOMPARE(documentData["offset"].toInt(), 0);
    QJsonArray projectsArray = json["projects"].toArray();
    QVERIFY(!projectsArray.isEmpty());
    QCOMPARE(projectsArray.count(), 18);
    QJsonObject project = projectsArray.at(1).toObject();
    QVERIFY(!project.isEmpty());
    const Task task = Redmine::Parser::parseProject(project);
    QCOMPARE(task.id(), 6);
    QCOMPARE(task.name(), QLatin1String("H1"));
    QCOMPARE(task.parent(), 2);
    QCOMPARE(task.validFrom().date(), QDate(2013, 4, 27));
}

void RedmineConnectorTests::testParseSampleIssueStatuses()
{
    using namespace Redmine;

    const QJsonObject json = testObject(":/RedmineConnectorTest/Data/RedmineConnector/SampleIssueStatuses.json");
    QJsonArray statusesArray = json["issue_statuses"].toArray();
    QVERIFY(!statusesArray.isEmpty());
    QCOMPARE(statusesArray.count(), 6);

    auto cmp = [](const Status& left, const Status& right) { return left.id() < right.id(); };
    std::set<Status, decltype(cmp)> statuses(cmp);
    auto parse_status = [](const QJsonValue& element) {
        auto status = Parser::parseStatus(element.toObject());
        Q_ASSERT(status.isValid());
        return status;
    };
    std::transform(statusesArray.begin(), statusesArray.end(), std::inserter(statuses, statuses.begin()), parse_status);
    const Status statusNew = *statuses.find(Status(1));
    QVERIFY(statusNew.isValid());
    QVERIFY(statusNew.isDefault());
    QVERIFY(!statusNew.isClosed());
    QCOMPARE(statusNew.name(), tr("New"));

    const Status statusClosed = *statuses.find(Status(5));
    QVERIFY(statusClosed.isValid());
    QVERIFY(!statusClosed.isDefault());
    QVERIFY(statusClosed.isClosed());
    QCOMPARE(statusClosed.name(), tr("Closed"));
}

QByteArray RedmineConnectorTests::testData(const QString &filename)
{
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        const QByteArray data(file.readAll());
        return data;
    }
    return QByteArray();
}

QJsonDocument RedmineConnectorTests::testDocument(const QString &filename)
{
    const QByteArray jsonData(testData(filename));
    if (jsonData.isEmpty()) {
        return QJsonDocument();
    }
    QJsonParseError jsonParseError;
    const QJsonDocument jsonDoc =QJsonDocument::fromJson(jsonData, &jsonParseError);
    if (jsonParseError.error != QJsonParseError::NoError) {
        return QJsonDocument();
    }
    return jsonDoc;
}

QJsonObject RedmineConnectorTests::testObject(const QString &filename)
{
    const QJsonDocument jsonDoc = testDocument(filename);
    if (jsonDoc.isNull()) {
        return QJsonObject();
    }
    return jsonDoc.object();
}

QTEST_MAIN(RedmineConnectorTests)

#include "RedmineConnectorTests.moc"
