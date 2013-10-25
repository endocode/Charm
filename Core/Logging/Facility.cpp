#include <QApplication>
#include <QtDebug>
#include <QDateTime>
#include <QDir>

#include <Core/CharmExceptions.h>

#include "Facility.h"
#include "Macros.h"
#include "Locations.h"

namespace Logging {

Facility* Facility::instance_;

Facility::Facility(QObject *parent)
    : QObject(parent)
    , loglevel_(LogLevel_Warn)
{
    Q_ASSERT(instance_ == 0);
    instance_ = this;
}

Facility::~Facility()
{
    Q_ASSERT(instance_ != 0);
    instance_ = 0;
}

void Facility::setLogfilePath(const QString &path)
{
    logfilePath_ = path;
}

QString Facility::logfilePath() const
{
    return logfilePath_;
}

void Facility::setLogLevel(Facility::LogLevel level)
{
    loglevel_ = level;
}

Facility::LogLevel Facility::logLevel() const
{
    return loglevel_;
}

/*! @brief Opens the log file, and redirects qDebug() output into it. */
void Facility::setupLogging(LogLevel level)
{
    if (logfilePath().isEmpty()) {
        setLogfilePath(Locations::logfilePath() + QDir::separator() + QCoreApplication::applicationName() + ".log");
    }
    Q_ASSERT(!logfilePath().isEmpty());
    setLogLevel(level);
    // Called *before* the message handler is installed, so that the user actually has a way to figure out
    // where the log file is located (for log levels > WARN).
    INFO(tr("Note: Log file is located at %1.").arg(logfilePath()));
    logfile_.setFileName(logfilePath());
    if(!logfile_.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append)) {
        qWarning().nospace() << "Cannot open log file \""
                             << qPrintable(logfilePath()) << "\": "
                             << qPrintable(logfile_.errorString());
    }
    qInstallMessageHandler(qDebugMsgHandler);
}

void Facility::trace(const QString &msg)
{
    message(LogLevel_Trace, msg);
}

void Facility::debug(const QString &msg)
{
    message(LogLevel_Debug, msg);
}

void Facility::info(const QString &msg)
{
    message(LogLevel_Info, msg);
}

void Facility::warn(const QString &msg)
{
    message(LogLevel_Warn, msg);
}

void Facility::error(const QString &msg)
{
    message(LogLevel_Error, msg);
}

void Facility::fatal(const QString &msg)
{
    message(LogLevel_Fatal, msg);
}

void Facility::message(Facility::LogLevel f, const QString &msg)
{
    if (instance_ != 0 && instance_->loglevel_ > f) {
        return;
    }

    QString facility;
    switch(f) {
    case LogLevel_Trace:
        facility = tr("TRACE");
        break;
    case LogLevel_Info:
        facility = tr("INFO");
        break;
    case LogLevel_Warn:
        facility = tr("WARN");
        break;
    case LogLevel_Error:
        facility = tr("ERROR");
        break;
    case LogLevel_Fatal:
        facility = tr("FATAL");
        break;
    case LogLevel_Debug:
    default:
        facility = tr("DEBUG");
        break;
    }
    Q_ASSERT(!facility.isEmpty());
    const QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
    QString text;
    {
        QTextStream stream(&text);
        const QString facilityString = tr("%1").arg(facility, 5);
        stream << qPrintable(timestamp) << " | " << facilityString << " | " << qPrintable(msg) << endl;
    }
    text = text.trimmed();
    if (!text.endsWith(QChar('.')) && !text.endsWith(QChar('!'))
            && !text.endsWith(QChar(':')) && !text.endsWith(QChar('?'))) {
            text.append('.');
    }
    QMutexLocker l(&instance_->mutex_);
    if (instance_ != 0 && instance_->logfile_.isOpen()) {
        QTextStream stream(&instance_->logfile_);
        stream << text << endl;
    } else {
        // fall back to regular output if no output file is set up
        const QByteArray characters = text.toLocal8Bit();
        fprintf(stderr, "%s\n", characters.constData());
    }
}

void Facility::qDebugMsgHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    switch (type) {
    case QtWarningMsg:
        warn(msg);
        break;
    case QtCriticalMsg:
        error(msg);
        break;
    case QtFatalMsg:
        fatal(msg);
        throw CharmException(msg);
        break;
    case QtDebugMsg:
    default:
        debug(msg);
    }
}

}
