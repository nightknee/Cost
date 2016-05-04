#include "baseexception.h"

BaseException::BaseException(QString msg)
{
    this->setMsg(msg);
}

BaseException::BaseException()
{

}

void BaseException::setMsg(QString msg){
    this->message = msg;
}

QString BaseException::getMsg(){
    return this->message;
}
