#ifndef BACKEND_H
#define BACKEND_H

#include <QCoreApplication>
#include <QString>

#include "Core/Task.h"
#include "Core/User.h"
#include "Core/State.h"
#include "Core/Event.h"
#include "Core/Installation.h"

class Event;
class Configuration;
class SqlRaiiTransactor;

#include "Core/StorageInterface.h"

namespace Redmine {

class Backend : public StorageInterface
{
    Q_DECLARE_TR_FUNCTIONS(Backend)
public:
    Backend();
    QString description() const override;
    void stateChanged(State previous) override;
    bool connect(Configuration&) override;
    bool disconnect() override;
    int installationId() const override;
    Installation getInstallation(int installationId) override;
    Installation createInstallation(const QString& name) override;
    bool modifyInstallation(const Installation&) override;
    bool deleteInstallation(const Installation&) override;
    User getUser(int userid) override;
    User makeUser(const QString& name) override;
    bool modifyUser(const User& user) override;
    bool deleteUser(const User& user) override;
    TaskList getAllTasks() override;
    bool setAllTasks( const User& user, const TaskList& tasks ) override;
    bool addTask(const Task& task) override;
    bool addTask( const Task& task, const SqlRaiiTransactor& ) override;
    Task getTask(int taskId) override;
    bool modifyTask(const Task& task) override;
    bool deleteTask(const Task& task) override;
    bool deleteAllTasks() override;
    bool deleteAllTasks( const SqlRaiiTransactor& ) override;
    EventList getAllEvents() override;
    Event makeEvent() override;
    Event makeEvent( const SqlRaiiTransactor& ) override;
    Event getEvent(int eventId)override;
    bool modifyEvent( const Event& event ) override;
    bool modifyEvent( const Event& event, const SqlRaiiTransactor& ) override;
    bool deleteEvent(const Event& event) override;
    bool deleteAllEvents() override;
    bool deleteAllEvents( const SqlRaiiTransactor& ) override;
    bool addSubscription(User, Task) override;
    bool deleteSubscription(User, Task) override;
    bool setMetaData(const QString& key, const QString& value) override;
    QString getMetaData(const QString& key) override;
    QString setAllTasksAndEvents( const User&, const TaskList&, const EventList& ) override;

protected:
    bool createDatabase(Configuration&) override;
    bool verifyDatabase() override;
};

}

#endif // BACKEND_H
