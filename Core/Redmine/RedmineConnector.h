#ifndef REDMINECONNECTOR_H
#define REDMINECONNECTOR_H

#include <QObject>
#include <Core/Task.h>

namespace Redmine {

class Connector : public QObject
{
    Q_OBJECT
public:
    explicit Connector(QObject *parent = 0);
    
    //temp, in lieu of a real API:
    TaskList buildTaskListFromFile(const QString& filename);

Q_SIGNALS:
    
public Q_SLOTS:

};

}

#endif // REDMINECONNECTOR_H
