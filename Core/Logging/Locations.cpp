#include <QStandardPaths>
#include <QDir>
#include <QCoreApplication>
#include <QtDebug>

#include <Core/CharmExceptions.h>

#include "Locations.h"
#include "Macros.h"

namespace Logging {

namespace Locations {

/*! @brief The path where the log file should be created.
  Defaults to QDesktopServices::TempLocation.
  The directory will be created if necessary. */
QString logfilePath()
{
    const QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    ensureDirectoryExists(path);
    return path;
}

/*! @brief Create the specified directory if needed.
  Throws a RedminerException on any error. */
void ensureDirectoryExists(const QString &path)
{
    Q_ASSERT(!QCoreApplication::applicationName().isEmpty());
    Q_ASSERT(!QCoreApplication::organizationName().isEmpty());
    Q_ASSERT(!QCoreApplication::organizationDomain().isEmpty());

    QDir dir(path);
    if (!dir.exists()) {
        if (dir.mkpath(path)) {
            DEBUG(QObject::tr("Created directory \"%1\"").arg(path));
        } else {
            const QString msg = QObject::tr("Unable to create directory \"%1\", aborting.").arg(path);
            ERROR(msg);
            throw CharmException(msg);
        }
    } else {
        DEBUG(QObject::tr("Directory \"%1\" already exists").arg(path));
    }
}

}

}
