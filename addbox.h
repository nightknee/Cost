#ifndef ADDBOX_H
#define ADDBOX_H

#include <QDialog>

#include "ui_dialog.h"
#include "sqlmanager.h"
#include "exceptions/setexistproductexception.h"

class AddBox : public QDialog, private Ui::Dialog
{
Q_OBJECT
public:
    AddBox();
    void addItemToComboBox(QString value, int id);
    QString getNewProductName();
    double getProductPrice();
    void resetDialog();
    int getSelectedProductId();
    QString getSelectedProductName();
    void setErrorLabelMsg(QString msg);
    void clearErrorLabel();
private:
    void hideElements();
    void addNewProduct();
    void addExistProduct();
private slots:
    void on_CancelButton_clicked();
    void on_checkBox_clicked();
    void on_AddButton_clicked();
signals:
    void addedNewProduct();
    void addedExistProduct();
};

#endif // ADDBOX_H
