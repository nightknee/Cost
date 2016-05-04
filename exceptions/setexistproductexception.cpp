#include "setexistproductexception.h"

SetExistProductException::SetExistProductException()
{

}

SetExistProductException::SetExistProductException(QString msg){
    this->setMsg(msg);
}

