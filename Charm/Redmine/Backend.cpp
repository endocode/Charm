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

}
