#ifndef STATUS_H
#define STATUS_H

#include <QString>

namespace Redmine {

/** @brief Status represents a Redmine issues status. */

class Status
{
public:
    typedef int Id;

    Status();
    Status(Id);

    bool isValid() const;

    Id id() const;
    void setId(Id);

    QString name() const;
    void setName(const QString&);

    bool isClosed() const;
    void setIsClosed(bool isit);

    bool isDefault() const;
    void setIsDefault(bool isit);

private:
    Id id_;
    QString name_;
    bool isDefault_, isClosed_;
};

}

#endif // STATUS_H
