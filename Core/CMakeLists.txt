SET(
    CharmCore_SRCS
    CharmConstants.cpp
    CharmExceptions.cpp
    Controller.cpp
    Dates.cpp
    SqlRaiiTransactor.cpp
    SqLiteStorage.cpp
    MySqlStorage.cpp
    Configuration.cpp
    SqlStorage.cpp
    Event.cpp
    Task.cpp
    TaskListMerger.cpp
    State.cpp
    CharmDataModel.cpp
    TaskTreeItem.cpp
    TimeSpans.cpp
    CharmCommand.cpp
    SmartNameCache.cpp
    XmlSerialization.cpp
    Logging/Locations.cpp
    Logging/Facility.cpp
    Redmine/RedmineModel.cpp
    Redmine/IssueStatus.cpp
    Redmine/RedmineProject.cpp
    Redmine/RedmineIssue.cpp
    Redmine/RedmineConnector.cpp
    Redmine/RedmineParser.cpp
    Redmine/RedmineConfiguration.cpp
    Redmine/RedmineRetriever.cpp
    Redmine/WindowRetriever.cpp
    Redmine/IssuesRetriever.cpp
    Redmine/ProjectsRetriever.cpp
    Redmine/RedmineTaskListProvider.cpp
    Redmine/RedmineUsersRetriever.cpp
    Redmine/CurrentUserRetriever.cpp
    Redmine/StatusRetriever.cpp
)

ADD_LIBRARY( CharmCore STATIC ${CharmCore_SRCS} )

TARGET_LINK_LIBRARIES( CharmCore ${QT_LIBRARIES} KF5::ThreadWeaver)
