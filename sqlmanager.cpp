#include "sqlmanager.h"
#include <QMessageBox>
SqlManager::SqlManager()
{
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName("data.db");
    if(this->db.open()){
        qDebug()<<"Open data.db";
    }else{
        qDebug()<<"Error open data.db";
    }
}

SqlManager::SqlManager(QString dbName)
{
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName(dbName);
    if(this->db.open()){
        qDebug()<<"Open data.db";
    }else{
        qDebug()<<"Error open data.db";
    }
}
void SqlManager::checkDb(){
    QSqlQuery query;
    this->prepareQueryStrings();
    if((!query.exec(*this->produktQueryCreate)) && (query.lastError().text() == "table Produkt already exists Unable to execute statement")){
        qDebug()<<"Produkt table exist";
    }else{
        qDebug()<<"Produkt table create";
    }
    if((!query.exec(*this->dataQueryCreate)) && (query.lastError().text() == "table Data already exists Unable to execute statement")){
        qDebug()<<"Data table exist";
    }else{
        qDebug()<<"Data table create";
    }
    if((!query.exec(*this->kosztQueryCreate)) && (query.lastError().text() == "table Koszt already exists Unable to execute statement")){
        qDebug()<<"Koszt table exist";
    }else{
        qDebug()<<"Koszt table create";
    }

    delete this->produktQueryCreate;
    delete this->dataQueryCreate;
    delete this->kosztQueryCreate;
}

void SqlManager::prepareQueryStrings(){
    this->produktQueryCreate = new QString("CREATE TABLE Produkt "
                                           "(Id INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , Name TEXT UNIQUE)");
    this->dataQueryCreate = new QString("CREATE TABLE Data "
                                        "(Id INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , "
                                        "CurrentData VARCHAR(10),"
                                        "Month VARCHAR(2),"
                                        "Year VARCHAR(10))");
    this->kosztQueryCreate = new QString("CREATE TABLE Koszt "
                                         "(Id INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , "
                                         "Rel_Data INT(10),"
                                         "Rel_Produkt INT(10),"
                                         "Koszt INT (10),"
                                         "FOREIGN KEY(Rel_Data) REFERENCES Data(Id),"
                                         "FOREIGN KEY(Rel_Produkt) REFERENCES Produkt(Id))");
}

void SqlManager::checkExistDateRow(QString* timeArray){
    QString *getCurrentDateRow = new QString("SELECT Id,CurrentData,Month,Year FROM Data WHERE CurrentData = '"+timeArray[0]+"' AND "
                                             "Month = '"+timeArray[1]+"' AND Year = '"+timeArray[2]+"'");
    QSqlQuery query;
    if((!query.exec(*getCurrentDateRow))){
         qDebug()<<"getCurrentDateRow"<<query.lastError().text();
    }
    query.next();
    if(query.isNull("CurrentData")){
        this->insertCurrentDate(timeArray);
    }

    delete getCurrentDateRow;
}

void SqlManager::insertCurrentDate(QString *timeArray){
    QString *insertCurrentDateRow = new QString("INSERT INTO Data (CurrentData,Month,Year) VALUES ('"+timeArray[0]+"','"+timeArray[1]+"','"+timeArray[2]+"')");
    QSqlQuery query;
    if((!query.exec(*insertCurrentDateRow))){
         qDebug()<<"insertCurrentDateRow"<<query.lastError().text();
    }
    delete insertCurrentDateRow;
}

QString SqlManager::getIdCurrentDate(QString currentDate){
    QString *getIdCurrentDate = new QString("SELECT Id FROM Data WHERE CurrentData = '"+currentDate+"'");
    QSqlQuery query;
    if((!query.exec(*getIdCurrentDate))){
         qDebug()<<"insertCurrentDateRow"<<query.lastError().text();
    }
    query.next();

    delete getIdCurrentDate;

    return query.value(0).toString();
}

QSqlQuery* SqlManager::getProductsFromCurrentDate(QString *timeArray){
    QSqlQuery *query = new QSqlQuery ;
    QString *getProductsCurrentDay = new QString("SELECT * FROM Koszt "
                                                 "LEFT JOIN Data ON Koszt.Rel_Data = Data.Id "
                                                 "LEFT JOIN Produkt ON Koszt.Rel_Produkt = Produkt.Id"
                                                 " WHERE Data.Month = "+timeArray[1]+" AND Data.Year = "+timeArray[2]);
    if((!query->exec(*getProductsCurrentDay))){
         qDebug()<<"getProductsFromCurrentDate"<<query->lastError().text();
    }
    delete getProductsCurrentDay;
    return query;
}

QSqlQuery* SqlManager::getProducts(){
    QSqlQuery *query = new QSqlQuery ;
    QString *getProducts = new QString("SELECT * FROM Produkt");

    if((!query->exec(*getProducts))){
         qDebug()<<"getProducts"<<query->lastError().text();
    }

    delete getProducts;
    return query;
}

QSqlQuery* SqlManager::getDates(){
    QSqlQuery *query = new QSqlQuery ;
    QString *getDates = new QString("SELECT Id,Month,Year FROM Data GROUP BY Month,Year");

    if((!query->exec(*getDates))){
         qDebug()<<"getDates"<<query->lastError().text();
    }

    delete getDates;
    return query;
}

bool SqlManager::addNewCost(QString productId, QString price, QString idDate){
    QSqlQuery *query = new QSqlQuery ;
    QString *addNewCost = new QString("INSERT INTO Koszt (Rel_Data,Rel_Produkt,Koszt) VALUES ('"+idDate+"','"+productId+"','"+price+"')");

    if((!query->exec(*addNewCost))){
         qDebug()<<"addNewCost"<<query->lastError().text();
         return false;
    }
    return true;
}



int SqlManager::addNewProduct(QString name){
    QSqlQuery *query = new QSqlQuery;
    QString *addNewProduct = new QString("INSERT INTO Produkt (Name) VALUES ('"+name+"')");
    bool requestResult = query->exec(*addNewProduct);
    if((!requestResult) && query->lastError().text() == "UNIQUE constraint failed: Produkt.Name Unable to fetch row"){
        throw new SetExistProductException("Produkt istnieje");
    }
    else if((!requestResult)){
        qDebug()<<"addNewProduct"<<query->lastError().text();
        qDebug()<<*addNewProduct;
        return 0;
    }else{
        return this->getIdProduct(name);
    }

    delete addNewProduct;
    delete query;
}

int SqlManager::getIdProduct(QString name){
    QString *getIdNewProduct = new QString("SELECT Id FROM Produkt WHERE Name = '"+name+"'");
    QSqlQuery query;
    if((!query.exec(*getIdNewProduct))){
         qDebug()<<"getIdNewProduct"<<query.lastError().text();
    }
    query.next();

    delete getIdNewProduct;

    return query.value(0).toInt();
}

bool SqlManager::removeProduct(int id){
    QSqlQuery *query = new QSqlQuery ;
    QString *removeCost = new QString("DELETE FROM Koszt WHERE Id = "+QString::number(id));

    if((!query->exec(*removeCost))){
         qDebug()<<"removeCost"<<query->lastError().text();
         return false;
    }
    return true;
}

float SqlManager::getProductPrice(int id){
    QString *getProductPrice = new QString("SELECT Koszt FROM Koszt WHERE Id = '"+QString::number(id)+"'");
    QSqlQuery query;
    if((!query.exec(*getProductPrice))){
         qDebug()<<"getProductPrice"<<query.lastError().text();
    }
    query.next();

    delete getProductPrice;

    return query.value(0).toFloat();
}

int SqlManager::getLastInsertCost(){
    QString *getLastInsertCost = new QString("SELECT * FROM Koszt ORDER BY Id DESC LIMIT 1");
    QSqlQuery query;
    if((!query.exec(*getLastInsertCost))){
         qDebug()<<"getIdNewProduct"<<query.lastError().text();
    }
    query.next();

    delete getLastInsertCost;

    return query.value(0).toInt();
}

QSqlQuery* SqlManager::getDateFromId(int id){
    qDebug()<<id;
    QString *getDateFromId = new QString("SELECT * FROM Data WHERE Id="+QString::number(id));
    QSqlQuery* query = new QSqlQuery;
    if((!query->exec(*getDateFromId))){
         qDebug()<<"getDateFromId"<<query->lastError().text();
    }
    query->next();
    delete getDateFromId;

    return query;
}


SqlManager::~SqlManager(){
    this->db.close();
}
