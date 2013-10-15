#include <QObject>
#include <QTextStream>

#include "CharmConstants.h"
#include "Controller.h"
#include "CharmDataModel.h"

const QString MetaKey_EventsInLeafsOnly = QLatin1String("EventsInLeafsOnly");
const QString MetaKey_OneEventAtATime = QLatin1String("OneEventAtATime");
const QString MetaKey_MainWindowGeometry = QLatin1String("MainWindowGeometry");
const QString MetaKey_MainWindowVisible = QLatin1String("MainWindowVisible");
const QString MetaKey_MainWindowGUIStateSelectedTask = QLatin1String("MainWindowGUIStateSelectedTask");
const QString MetaKey_MainWindowGUIStateExpandedTasks = QLatin1String("MainWindowGUIStateExpandedTasks");
const QString MetaKey_TimeTrackerGeometry = QLatin1String("TimeTrackerGeometry");
const QString MetaKey_TimeTrackerVisible = QLatin1String("TimeTrackerVisible");
const QString MetaKey_ReportsRecentSavePath = QLatin1String("ReportsRecentSavePath");
const QString MetaKey_ExportToXmlRecentSavePath = QLatin1String("ExportToXmlSavePath");
const QString MetaKey_TimesheetSubscribedOnly = QLatin1String("TimesheetSubscribedOnly");
const QString MetaKey_TimesheetActiveOnly = QLatin1String("TimesheetActiveOnly");
const QString MetaKey_TimesheetRootTask = QLatin1String("TimesheetRootTask");
const QString MetaKey_LastEventEditorDateTime = QLatin1String("LastEventEditorDateTime");
const QString MetaKey_Key_InstallationId = QLatin1String("InstallationId");
const QString MetaKey_Key_UserName = QLatin1String("UserName");
const QString MetaKey_Key_UserId = QLatin1String("UserId");
const QString MetaKey_Key_LocalStorageDatabase = QLatin1String("LocalStorageDatabase");
const QString MetaKey_Key_LocalStorageType = QLatin1String("LocalStorageType");
const QString MetaKey_Key_SubscribedTasksOnly = QLatin1String("SubscribedTasksOnly");
const QString MetaKey_Key_TimeTrackerFontSize = QLatin1String("TimeTrackerFontSize");
const QString MetaKey_Key_24hEditing = QLatin1String("Key24hEditing");
const QString MetaKey_Key_DurationFormat = QLatin1String("DurationFormat");
const QString MetaKey_Key_IdleDetection = QLatin1String("IdleDetection");
const QString MetaKey_Key_WarnUnuploadedTimesheets = QLatin1String("WarnUnuploadedTimesheets");
const QString MetaKey_Key_ToolButtonStyle = QLatin1String("ToolButtonStyle");
const QString MetaKey_Key_ShowStatusBar = QLatin1String("ShowStatusBar");

const QString TrueString = QLatin1String( "true" );
const QString FalseString = QLatin1String( "false" );

const QString& stringForBool( bool val )
{
    return val ? TrueString : FalseString;
}

void connectControllerAndModel( Controller* controller, CharmDataModel* model )
{
    QObject::connect( controller, SIGNAL( eventAdded( const Event& ) ),
                      model, SLOT( addEvent( const Event& ) ) );
    QObject::connect( controller, SIGNAL( eventModified( const Event& ) ),
                      model, SLOT( modifyEvent( const Event& ) ) );
    QObject::connect( controller, SIGNAL( eventDeleted( const Event& ) ),
                      model, SLOT( deleteEvent( const Event& ) ) );
    QObject::connect( controller, SIGNAL( allEvents( const EventList& ) ),
                      model, SLOT( setAllEvents( const EventList& ) ) );
    QObject::connect( controller, SIGNAL( definedTasks( const TaskList& ) ),
                      model, SLOT( setAllTasks( const TaskList& ) ) );
    QObject::connect( controller, SIGNAL( taskAdded( const Task& ) ),
                      model, SLOT( addTask( const Task& ) ) );
    QObject::connect( controller, SIGNAL( taskUpdated( const Task& ) ),
                      model, SLOT( modifyTask( const Task& ) ) );
    QObject::connect( controller, SIGNAL( taskDeleted( const Task& ) ),
                      model, SLOT( deleteTask( const Task& ) ) );
}

static QString formatDecimal( double d ) {
    const QString s = QLocale::system().toString( d, 'f', 2 );
    if ( d > -10 && d < 10 ) //hack to get the hours always have two decimals: e.g. 00.50 instead of 0.50
        return QLatin1String("0") + s;
    else
        return s;
}

QString hoursAndMinutes( int duration )
{
    if ( duration == 0 ) {
        if ( CONFIGURATION.durationFormat == Configuration::Minutes )
            return QObject::tr( "00:00" );
        else
            return formatDecimal( 0.0 );
    }
    int minutes = duration / 60;
    int hours = minutes / 60;
    minutes = minutes % 60;

    if ( CONFIGURATION.durationFormat == Configuration::Minutes ) {
        QString text;
        QTextStream stream( &text );
        stream << qSetFieldWidth( 2 ) << qSetPadChar( QChar( '0' ) )
                << hours << qSetFieldWidth( 0 ) << ":" << qSetFieldWidth( 2 ) << minutes;
        return text;
    } else { //Decimal
        return formatDecimal(hours + minutes / 60.0  );
    }
}
