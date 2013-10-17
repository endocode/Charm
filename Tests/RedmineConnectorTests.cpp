#include <QString>
#include <QtTest/QtTest>
#include <QCoreApplication>
#include <QJsonDocument>
#include <Core/Redmine/RedmineConnector.h>
#include <Core/Task.h>

class RedmineConnectorTests : public QObject
{
    Q_OBJECT
public:
    RedmineConnectorTests();
    
private Q_SLOTS:
    void testParseSampleIssue217();
    void testParseSample3Issues();
    void testParseSample18Projects();
    void testBuildTaskList();

private:
    QByteArray testData(const QString& filename);
    QJsonDocument testDocument(const QString& filename);
    QJsonObject testObject(const QString& filename);
};

RedmineConnectorTests::RedmineConnectorTests()
{
}

void RedmineConnectorTests::testParseSampleIssue217()
{
    const QJsonObject json = testObject(":/RedmineConnectorTest/Data/RedmineConnector/SampleIssue271.json");
    QVERIFY(!json.isEmpty());
    const QJsonValue issue = json[QString("issue")];
    QVERIFY(!issue.isNull());
    QVERIFY(issue.isObject());
    const QVariantMap issueData = issue.toObject().toVariantMap();
    const QDateTime updatedOnTime(QDateTime::fromString(issueData["updated_on"].toString(), Qt::ISODate));
    QCOMPARE(updatedOnTime.date(), QDate(2013, 8, 21));
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
    Redmine::Connector connector;
    Task task = connector.parseProject(project);
    QCOMPARE(task.id(), 6);
    QCOMPARE(task.name(), QLatin1String("H1"));
    QCOMPARE(task.parent(), 2);
    QCOMPARE(task.validFrom().date(), QDate(2013, 4, 27));
}

void RedmineConnectorTests::testBuildTaskList()
{
    const QJsonObject json = testObject(":/RedmineConnectorTest/Data/RedmineConnector/Sample18Projects.json");
    QVERIFY(!json.isEmpty());
    QJsonArray projectsArray = json["projects"].toArray();
    QCOMPARE(projectsArray.count(), 18);

    Redmine::Connector redmine;
    TaskList tasks = redmine.buildTaskList(projectsArray, QJsonArray());
    QCOMPARE(tasks.count(), projectsArray.count());
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
