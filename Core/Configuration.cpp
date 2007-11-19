#include <QtDebug>
#include <QSettings>

#include "Configuration.h"
#include "CharmConstants.h"

#ifdef NDEBUG
#define DEFAULT_CONFIG_GROUP "default"
#else
#define DEFAULT_CONFIG_GROUP "debug"
#endif

Configuration& Configuration::instance()
{
    static Configuration configuration;
    return configuration;
}

Configuration::Configuration()
    : eventsInLeafsOnly( true )
    , oneEventAtATime( true )
    , showOnlySubscribedTasks( false )
    , taskTrackerFontSize( TaskTrackerFont_Regular )
    , always24hEditing( false )
    , configurationName( DEFAULT_CONFIG_GROUP )
    , installationId( 0 )
    , newDatabase( false )
    , failure( false )
    , taskPaddingLength( 6 ) // arbitrary
{
}

void Configuration::writeTo( QSettings& settings )
{
    settings.setValue( MetaKey_Key_InstallationId, installationId );
    settings.setValue( MetaKey_Key_UserId, user.id() );
    settings.setValue( MetaKey_Key_LocalStorageType, localStorageType );
    settings.setValue( MetaKey_Key_LocalStorageDatabase, localStorageDatabase );
    settings.setValue( MetaKey_Key_SubscribedTasksOnly, showOnlySubscribedTasks );
    settings.setValue( MetaKey_Key_TaskTrackerFontSize, static_cast<int>( taskTrackerFontSize ) );
    settings.setValue( MetaKey_Key_24hEditing, always24hEditing );
    dump( "(Configuration::writeTo stored configuration)" );
}

bool Configuration::readFrom( QSettings& settings )
{
    bool complete = true;
    if ( settings.contains( MetaKey_Key_InstallationId ) ) {
        installationId = settings.value( MetaKey_Key_InstallationId ).toInt();
    } else {
        complete = false;
    }
    if ( settings.contains( MetaKey_Key_UserId ) ) {
        user.setId( settings.value( MetaKey_Key_UserId ).toInt() );
    } else {
        complete = false;
    }
    if ( settings.contains( MetaKey_Key_LocalStorageType ) ) {
        localStorageType = settings.value( MetaKey_Key_LocalStorageType ).toString();
    } else {
        complete = false;
    }
    if ( settings.contains( MetaKey_Key_LocalStorageDatabase ) ) {
        localStorageDatabase = settings.value( MetaKey_Key_LocalStorageDatabase ).toString();
    } else {
        complete = false;
    }
    // ----- optional settings:
    if ( settings.contains( MetaKey_Key_SubscribedTasksOnly ) ) {
        showOnlySubscribedTasks = settings.value( MetaKey_Key_SubscribedTasksOnly ).value<bool>();
    }
    if ( settings.contains( MetaKey_Key_TaskTrackerFontSize ) ) {
        int setting = settings.value( MetaKey_Key_TaskTrackerFontSize ).value<int>();
        taskTrackerFontSize = static_cast<TaskTrackerFontSize>( setting );
    }
    if ( settings.contains( MetaKey_Key_24hEditing ) ) {
        always24hEditing = settings.value( MetaKey_Key_24hEditing ).value<bool>();
    }

    dump( "(Configuration::readFrom loaded configuration)" );
    return complete;
}

void Configuration::dump( const QString& why )
{
    // dump configuration:
    return; // disable debug output
    qDebug() << "Application::enterStartingUpState: configuration:"
             << ( why.isEmpty() ? QString() : why )
             << endl
             << "--> installation id:        " << installationId << endl
             << "--> userid:                 " << user.id() << endl
             << "--> local storage type:     " << localStorageType << endl
             << "--> local storage database: " << localStorageDatabase << endl
             << "--> subscribed tasks only:  " << showOnlySubscribedTasks << endl
             << "--> task tracker font size: " << taskTrackerFontSize
             << "--> 24h time editing:       " << always24hEditing;
}

