#ifndef BACKEND_H
#define BACKEND_H

#include <QCoreApplication>

#include "Core/StorageInterface.h"

namespace Redmine {

class Backend : public StorageInterface
{
    Q_DECLARE_TR_FUNCTIONS(Backend)
public:
    Backend();
    QString description() const override;

};

}

#endif // BACKEND_H
