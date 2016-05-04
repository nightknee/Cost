#ifndef SETEXISTPRODUCTEXCEPTION_H
#define SETEXISTPRODUCTEXCEPTION_H
#include "baseexception.h"

class SetExistProductException : public BaseException
{
public:
    SetExistProductException();
    SetExistProductException(QString msg);
};

#endif // SETEXISTPRODUCTEXCEPTION_H
