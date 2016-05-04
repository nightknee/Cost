#ifndef BASEEXCEPTION_H
#define BASEEXCEPTION_H

#include <QString>

class BaseException
{
public:
    BaseException();
    BaseException(QString msg);
    void setMsg(QString msg);
    QString getMsg();

private:
    QString message;
};

#endif // BASEEXCEPTION_H
