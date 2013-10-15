#include <QString>
#include <QtTest/QtTest>
#include <QCoreApplication>
#include <QJsonDocument>
#include <Core/Task.h>

class RedmineConnectorTests : public QObject
{
    Q_OBJECT
public:
    RedmineConnectorTests();
    
private Q_SLOTS:
    void testParseSampleIssue217();
};

RedmineConnectorTests::RedmineConnectorTests()
{
}

void RedmineConnectorTests::testParseSampleIssue217()
{
    QFile jsonFile(":/RedmineConnectorTest/Data/RedmineConnector/SampleIssue271.json");
    QVERIFY(jsonFile.open(QIODevice::ReadOnly));
    const QByteArray jsonData(jsonFile.readAll());
    QVERIFY(!jsonData.isEmpty());
    QJsonParseError jsonParseError;
    const QJsonDocument jsonDoc =QJsonDocument::fromJson(jsonData, &jsonParseError);
    QVERIFY(jsonParseError.error == QJsonParseError::NoError);
    QVERIFY(!jsonDoc.isNull());
    const QJsonObject json(jsonDoc.object());
    QVERIFY(!json.isEmpty());
    const QJsonValue issue = json[QString("issue")];
    QVERIFY(!issue.isNull());
    QVERIFY(issue.isObject());
    const QVariantMap issueData = issue.toObject().toVariantMap();
    QDateTime updatedOnTime(QDateTime::fromString(issueData["updated_on"].toString(), Qt::ISODate));
    QCOMPARE(updatedOnTime.date(), QDate(2013, 8, 21));
    Task task;
}

QTEST_MAIN(RedmineConnectorTests)

#include "RedmineConnectorTests.moc"
