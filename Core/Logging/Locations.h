#ifndef LOCATIONS_H
#define LOCATIONS_H

namespace Logging {

namespace Locations {

QString logfilePath();

void ensureDirectoryExists(const QString& path);

}

}

#endif // LOCATIONS_H
