#ifndef LOGGING_FACILITY_H
#define LOGGING_FACILITY_H

#include <QObject>
#include <QFile>
#include <QMutex>

namespace Logging {

/*! @brief Facility configures the logging of an application.
  It configures log file output and handles logging according to the debug level set in the configuration.
  It also installs hooks for the general qDebug handlers to reroute qDebug output into the same file.
*/
class Facility : public QObject
{
    Q_OBJECT
public:
    enum LogLevel {
        LogLevel_Trace,
        LogLevel_Debug,
        LogLevel_Info,
        LogLevel_Warn,
        LogLevel_Error,
        LogLevel_Fatal
    };

    explicit Facility(QObject *parent = 0);
    virtual ~Facility();

    void setLogfilePath(const QString& path);
    QString logfilePath() const;

    void setLogLevel(LogLevel level);
    LogLevel logLevel() const;

    void setupLogging(LogLevel level = LogLevel_Warn);


    static void trace(const QString& msg);
    static void debug(const QString& msg);
    static void info(const QString& msg);
    static void warn(const QString& msg);
    static void error(const QString& msg);
    static void fatal(const QString& msg);
    static void message(LogLevel f, const QString& msg);

private:
    static Facility* instance_;
    QString logfilePath_;
    QFile logfile_;
    LogLevel loglevel_;
    QMutex mutex_;
    static void qDebugMsgHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};

}

#endif // LOGGING_FACILITY_H
