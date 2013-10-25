#include "Status.h"

namespace Redmine {

Status::Status()
    : Status(Status::Id())
{
}

Status::Status(Status::Id id)
    : id_(id)
    , isDefault_(false)
    , isClosed_(false)
{
}

bool Status::isValid() const
{
    return id_ != 0;
}

Status::Id Status::id() const
{
    return id_;
}

void Status::setId(Status::Id id)
{
    id_ = id;
}

QString Status::name() const
{
    return name_;
}

void Status::setName(const QString& name)
{
    name_ = name;
}

bool Status::isClosed() const
{
    return isClosed_;
}

void Status::setIsClosed(bool isit)
{
    isClosed_ = isit;
}

bool Status::isDefault() const
{
    return isDefault_;
}

void Status::setIsDefault(bool isit)
{
    isDefault_ = isit;
}

}
