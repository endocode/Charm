#include <QApplication>

#include "Backend.h"

namespace Redmine {

Backend::Backend()
{
}

QString Backend::description() const
{
    return tr("Redmine Server");
}

void Backend::stateChanged(State previous)
{
    //NI
}

bool Backend::connect(Configuration &)
{
    //NI
    return false;
}

bool Backend::disconnect()
{
    //NI
    return false;
}

int Backend::installationId() const
{
    //NI
    return false;
}

Installation Backend::getInstallation(int installationId)
{
    //NI
    return Installation();
}

Installation Backend::createInstallation(const QString &name)
{
    //NI
    return Installation();
}

bool Backend::modifyInstallation(const Installation &)
{
    //NI
    return false;
}

bool Backend::deleteInstallation(const Installation &)
{
    //NI
    return false;
}

User Backend::getUser(int userid)
{
    //NI
    return User();
}

User Backend::makeUser(const QString &name)
{
    //NI
    return User();
}

bool Backend::modifyUser(const User &user)
{
    //NI
    return false;
}

bool Backend::deleteUser(const User &user)
{
    //NI
    return false;
}

TaskList Backend::getAllTasks()
{
    //NI
    return TaskList();
}

bool Backend::setAllTasks(const User &user, const TaskList &tasks)
{
    //NI
    return false;
}

bool Backend::addTask(const Task &task)
{
    //NI
    return false;
}

bool Backend::addTask(const Task &task, const SqlRaiiTransactor &)
{
    //NI
    return false;
}

Task Backend::getTask(int taskId)
{
    //NI
    return Task();
}

bool Backend::modifyTask(const Task &task)
{
    //NI
    return false;
}

bool Backend::deleteTask(const Task &task)
{
    //NI
    return false;
}

bool Backend::deleteAllTasks()
{
    //NI
    return false;
}

bool Backend::deleteAllTasks(const SqlRaiiTransactor &)
{
    //NI
    return false;
}

EventList Backend::getAllEvents()
{
    //NI
    return EventList();
}

Event Backend::makeEvent()
{
    //NI
    return Event();
}

Event Backend::makeEvent(const SqlRaiiTransactor &)
{
    //NI
    return Event();
}

Event Backend::getEvent(int eventId)
{
    //NI
    return Event();
}

bool Backend::modifyEvent(const Event &event)
{
    //NI
    return false;
}

bool Backend::modifyEvent(const Event &event, const SqlRaiiTransactor &)
{
    //NI
    return false;
}

bool Backend::deleteEvent(const Event &event)
{
    //NI
    return false;
}

bool Backend::deleteAllEvents()
{
    //NI
    return false;
}

bool Backend::deleteAllEvents(const SqlRaiiTransactor &)
{
    //NI
    return false;
}

bool Backend::addSubscription(User, Task)
{
    //NI
    return false;
}

bool Backend::deleteSubscription(User, Task)
{
    //NI
    return false;
}

bool Backend::setMetaData(const QString &key, const QString &value)
{
    //NI
    return false;
}

QString Backend::getMetaData(const QString &key)
{
    //NI
    return tr("NI");
}

QString Backend::setAllTasksAndEvents(const User &, const TaskList &, const EventList &)
{
    //NI
    return tr("NI");
}

bool Backend::createDatabase(Configuration &)
{
    //NI
    return false;
}

bool Backend::verifyDatabase()
{
    //NI
    return false;
}

}
