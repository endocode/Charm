#ifndef SQLTRANSACTIONTESTS_H
#define SQLTRANSACTIONTESTS_H

#include <QObject>

#include "Core/MySqlStorage.h"

class SqlTransactionTests : public QObject
{
    Q_OBJECT

public:
    SqlTransactionTests();

private Q_SLOTS:
    void testMySqlDriverRequirements();
    void testSqLiteDriverRequirements();
    void testMySqlTransactionRollback();
    void testMySqlTransactionCommit();
    void testMySqlNestedTransactions();

private:
    MySqlStorage prepareMySqlStorage();
};


#endif // SQLTRANSACTIONTESTS_H
