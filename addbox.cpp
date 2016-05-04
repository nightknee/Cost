#include "addbox.h"
AddBox::AddBox()

{
    this->setupUi(this);

    this->hideElements();
}



void AddBox::on_CancelButton_clicked()
{
    this->hide();
}

void AddBox::on_checkBox_clicked()
{
    if(this->checkBox->isChecked()){
        this->newProduktName->show();
    }
    else{
        this->newProduktName->close();
    }
}

void AddBox::hideElements(){
    this->newProduktName->hide();
}

void AddBox::addItemToComboBox(QString value,int id){
    this->existProducts->addItem(value,QVariant(id));
}

void AddBox::on_AddButton_clicked()
{
    if(this->checkBox->isChecked()){
        this->addNewProduct();
    }else{
        this->addExistProduct();
    }
}

void AddBox::addNewProduct(){
    QString *productName = new QString(this->newProduktName->text());
    double price = this->price->value();
    if(!productName->isEmpty() && price){
        emit addedNewProduct();
    }
}

QString AddBox::getNewProductName(){
    return this->newProduktName->text();
}

double AddBox::getProductPrice(){
    return this->price->value();
}

void AddBox::resetDialog(){
    this->hideElements();
    this->checkBox->setChecked(false);
    this->newProduktName->clear();
    this->price->setValue(0.00);
}

void AddBox::addExistProduct(){
    double *price = new double(this->price->value());
    int *idSelect = new int(this->getSelectedProductId());
    if(*price && *idSelect){
        emit addedExistProduct();
    }
    delete price;
    delete idSelect;
}

int AddBox::getSelectedProductId(){
    return this->existProducts->itemData(this->existProducts->currentIndex()).toInt();
}

void AddBox::setErrorLabelMsg(QString msg){
    this->errorLabel->setText(msg);
}

void AddBox::clearErrorLabel(){
    this->errorLabel->clear();
}

QString AddBox::getSelectedProductName(){
     return this->existProducts->currentText();
}

