#ifndef LOGGINGMACROS_H
#define LOGGINGMACROS_H

#include <Core/Logging/Facility.h>

/*
These macros potentially clash with macros defined by other projects.
Avoid including this header file in other header files. Use it in .cpp files only.
*/

#define TRACE(x) (Logging::Facility::trace(x))
#define DEBUG(x) (Logging::Facility::debug(x))
#define INFO(x) (Logging::Facility::info(x))
#define WARN(x) (Logging::Facility::warn(x))
#define ERROR(x) (Logging::Facility::error(x))
#define FATAL(x) (Logging::Facility::fatal(x))

#endif // LOGGINGMACROS_H
