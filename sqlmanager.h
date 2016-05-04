#ifndef SQLMANAGER_H
#define SQLMANAGER_H

#include <QtSql>
#include <QString>
#include <QMessageBox>
#include <QDateTime>


#include "exceptions/setexistproductexception.h"

class SqlManager
{
public:
    SqlManager();
    SqlManager(QString dbName);
    void checkDb();
    void checkExistDateRow(QString *timeArray);
    QString getIdCurrentDate(QString currentDate);    
    QSqlQuery *getProductsFromCurrentDate(QString *timeArray);
    QSqlQuery *getProducts();
    QSqlQuery *getDates();
    QSqlQuery *getDateFromId(int id);
    bool addNewCost(QString productId, QString price, QString idDate);
    int addNewProduct(QString name);
    int getIdProduct(QString name);
    bool removeProduct(int id);
    float getProductPrice(int id);
    int getLastInsertCost();
    ~SqlManager();
private:
    QSqlDatabase db;
    QString fileName;
    QString *produktQueryCreate;
    QString *dataQueryCreate;
    QString *kosztQueryCreate;
    void insertCurrentDate(QString *timeArray);

    void prepareQueryStrings();

};

#endif // SQLMANAGER_H
