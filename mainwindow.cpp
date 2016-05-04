#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /*Prepare UI and Tableviews*/
    ui->setupUi(this);
    this->dialog = new AddBox;
    this->dialog->hide();
    this->sql = new SqlManager;

    this->prepareDb();
    this->prepareTodayTable();
    this->prepareHistoryTable();

    /*Check current date and get correct rows*/
    this->checkCurrentDate();
    this->setTitleLabel();
    this->getProductsWithCurrentDate();

    /*Insert mounth in combo box from history tab*/
    this->insertMounthInHistoryTab();

    /*Put products in widget*/
    this->insertProductsInTodayTable();

    /*Prepare dialog window*/
    this->prepareAddBox();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_AddButton_clicked()
{
   this->dialog->exec();
}

void MainWindow::prepareDb(){
    try{
        this->sql->checkDb();
    }
    catch(QString *e){

        qDebug()<<*e;
    }
}

void MainWindow::prepareTodayTable(){
    this->ui->TodayTable->setShowGrid(false);
    this->ui->TodayTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->TodayTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->TodayTable->verticalHeader()->setVisible(false);
    this->ui->TodayTable->setColumnHidden(2,true);
    this->setSelectedRow(-1);
}

void MainWindow::prepareHistoryTable(){
    this->ui->HistoryTable->setShowGrid(false);
    this->ui->HistoryTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->HistoryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->HistoryTable->verticalHeader()->setVisible(false);
    this->ui->HistoryTable->setColumnHidden(2,true);
}

void MainWindow::checkCurrentDate(){
    QString *_currentDate;
    QString *_currentYear;
    QString *_currentMonth;
    QDate *date = new QDate;
    QString *str;

    str = new QString("d.M.yyyy");
    _currentDate = new QString(date->currentDate().toString(*str));

    *str = "yyyy";
    _currentYear = new QString(date->currentDate().toString(*str));

    *str = "M";
    _currentMonth = new QString(date->currentDate().toString(*str));


    this->setCurrentDate(*_currentDate);
    this->setCurrentYear(*_currentYear);
    this->setCurrentMonth(*_currentMonth);

    this->setTimeArray();

    delete date;
    delete str;
    delete _currentDate;
    delete _currentMonth;
    delete _currentYear;
}

void MainWindow::getProductsWithCurrentDate(){    
    this->sql->checkExistDateRow(this->getTimeArray());
    QString *_idCurrentDate = new QString(this->sql->getIdCurrentDate(this->getCurrentDate()));

    this->setIdCurrentDate(*_idCurrentDate);

    QSqlQuery *productsInCurrentMonth;

    productsInCurrentMonth = this->sql->getProductsFromCurrentDate(this->getTimeArray());
    this->setProductsCurrentDate(productsInCurrentMonth);

    delete _idCurrentDate;
}

QString MainWindow::getCurrentDate(){
    return this->currentDate;
}

void MainWindow::setIdCurrentDate(QString IdCurrentDate){
    this->idCurrentDate = IdCurrentDate;
}

QString MainWindow::getIdCurrentDate(){
    return this->idCurrentDate;
}

void MainWindow::setCurrentDate(QString newCurrentDate){
    this->currentDate = newCurrentDate;
}

void MainWindow::setProductsCurrentDate(QSqlQuery *IdCurrentDate){
    this->productsCurrentDate = IdCurrentDate;
}

QSqlQuery* MainWindow::getProductsCurrentDate(){
    return this->productsCurrentDate;
}

void MainWindow::insertProductsInTodayTable(){
    float priceMonth = 0;
    while(this->productsCurrentDate->next()){
        this->addRowInTodayTable(this->productsCurrentDate->value("Name").toString(),this->productsCurrentDate->value("Koszt").toString()+" zł",
                     this->productsCurrentDate->value("Id").toString());
        priceMonth+=this->productsCurrentDate->value("Koszt").toFloat();
    }
    this->setPriceInMonth(priceMonth);
}

void MainWindow::addRowInTodayTable(QString name,QString price,QString Id){
    int row;
    row = this->ui->TodayTable->rowCount();
    this->ui->TodayTable->insertRow(row);

    this->ui->TodayTable->setItem(row,0, new QTableWidgetItem(name));
    this->ui->TodayTable->setItem(row,1, new QTableWidgetItem(price));
    this->ui->TodayTable->setItem(row,2, new QTableWidgetItem(Id));
}

void MainWindow::addRowInHistoryTable(QString name,QString price,QString Id){
    int row;
    row = this->ui->HistoryTable->rowCount();
    this->ui->HistoryTable->insertRow(row);

    this->ui->HistoryTable->setItem(row,0, new QTableWidgetItem(name));
    this->ui->HistoryTable->setItem(row,1, new QTableWidgetItem(price));
    this->ui->HistoryTable->setItem(row,2, new QTableWidgetItem(Id));
}

void MainWindow::setPriceInMonth(float price){
    this->setPriceInCurrentDate(price);
    this->ui->CostInCurrentMonth->setText(QString::number(price)+" zł");
}

void MainWindow::prepareAddBox(){
    this->AddBox_addProductsToComboBox();
    this->connect(this->dialog,SIGNAL(addedNewProduct()),this,SLOT(slotAddedNewProduct()));
    this->connect(this->dialog,SIGNAL(addedExistProduct()),this,SLOT(slotAddedExistProduct()));
}

void MainWindow::setCurrentYear(QString currentYear){
    this->currentYear = currentYear;
}

QString MainWindow::getCurrentYear(){
    return this->currentYear;
}

void MainWindow::setCurrentMonth(QString currentMonth){
    this->currentMonth = currentMonth;
}

QString MainWindow::getCurrentMonth(){
    return this->currentMonth;
}

QString* MainWindow::getTimeArray(){
    return this->timeArray;
}

void MainWindow::setTimeArray(){
    this->timeArray = new QString[3];
    this->timeArray[0] = this->getCurrentDate();
    this->timeArray[1] = this->getCurrentMonth();
    this->timeArray[2] = this->getCurrentYear();
}

void MainWindow::AddBox_addProductsToComboBox(){
    this->products = this->sql->getProducts();
    while(this->products->next()){
        this->dialog->addItemToComboBox(this->products->value("Name").toString(),this->products->value("Id").toInt());
    }
}

void MainWindow::slotAddedNewProduct(){
    try{
        float *price = new float(this->dialog->getProductPrice());
        QString *productName = new QString(this->dialog->getNewProductName());

        int *idProduct = new int(this->sql->addNewProduct(*productName));

        if(*idProduct){
            this->sql->addNewCost(QString::number(*idProduct),QString::number(*price),this->getIdCurrentDate());
            int *idLast = new int(this->sql->getLastInsertCost());

            this->setRowToTodayTable(*productName,*price,*idLast);
            this->dialog->addItemToComboBox(*productName,*idProduct);
            this->dialog->resetDialog();
            this->dialog->clearErrorLabel();
        }else{
            qDebug()<<*idProduct;
        }
        delete price;
        delete productName;
        delete idProduct;
    }
    catch(SetExistProductException *e){
        this->dialog->setErrorLabelMsg("Taki produkt już istnieje");
        this->dialog->resetDialog();
    }
}

void MainWindow::slotAddedExistProduct(){
   int *idProduct = new int(this->dialog->getSelectedProductId());
   float *productPrice = new float(this->dialog->getProductPrice());
   QString *productName = new QString(this->dialog->getSelectedProductName());

   this->sql->addNewCost(QString::number(*idProduct),QString::number(*productPrice),this->getIdCurrentDate());

   int *idLast = new int(this->sql->getLastInsertCost());

   this->setRowToTodayTable(*productName,*productPrice,*idLast);
   this->dialog->resetDialog();
   this->dialog->clearErrorLabel();

   delete idProduct;
   delete idLast;
   delete productPrice;
   delete productName;
}

void MainWindow::setRowToTodayTable(QString productName,float price,int id){
    this->addRowInTodayTable(productName,QString::number(price)+" zł",QString::number(id));

    float *_newPrice = new float(this->getPriceInCurrentDate());
    *_newPrice+=price;

    this->setPriceInCurrentDate(*_newPrice);
    this->ui->CostInCurrentMonth->setText(QString::number(*_newPrice)+" zł");

    delete _newPrice;
}

void MainWindow::setPriceInCurrentDate(float price){
    this->priceInCurrentDate = price;
}

float MainWindow::getPriceInCurrentDate(){
    return this->priceInCurrentDate;
}

void MainWindow::setSelectedRow(int row){
    this->selectedRow = row;
}

int MainWindow::getSelectedRow(){
    return this->selectedRow;
}

void MainWindow::on_DeleteRow_clicked()
{
    float *productPrice = new float(this->sql->getProductPrice(this->getIdSqlSelectedCostRow()));

    if(this->sql->removeProduct(this->getIdSqlSelectedCostRow())){
        float *_cost = new float(this->getPriceInCurrentDate());
        *_cost-=*productPrice;
        this->setPriceInCurrentDate(*_cost);
        this->ui->CostInCurrentMonth->setText(QString::number(*_cost)+" zł");

        this->ui->TodayTable->removeRow(this->getSelectedRow());
    }
}

void MainWindow::on_TodayTable_clicked(const QModelIndex &index)
{
    int *row = new int(index.row());
    int *id = new int(index.sibling(*row,2).data().toInt());

    this->setSelectedRow(*row);
    this->setIdSqlSelectedCostRow(*id);

    delete row;
    delete id;
}

void MainWindow::setIdSqlSelectedCostRow(int id){
    this->idSqlSelectedCostRow = id;
}

int MainWindow::getIdSqlSelectedCostRow(){
    return this->idSqlSelectedCostRow;
}

void MainWindow::setTitleLabel(){
    QString *str = new QString;
    *str = this->getCurrentMonth()+"-"+this->getCurrentYear();

    this->ui->titleLabel->setText(*str);

    delete str;
}

void MainWindow::insertMounthInHistoryTab(){
    QSqlQuery* dates = this->sql->getDates();
    while(dates->next()){
        this->ui->MounthHistory->addItem(dates->value(1).toString()+"-"+dates->value(2).toString(),QVariant(dates->value(0).toInt()));
    }
}

void MainWindow::on_MounthHistory_currentIndexChanged(int index)
{
    int *idSelectedDate = new int(this->ui->MounthHistory->itemData(this->ui->MounthHistory->currentIndex()).toInt());
    if(*idSelectedDate != 0){
        this->insertProductsInHistoryTable(*idSelectedDate);
    }

    delete idSelectedDate;
}

void MainWindow::insertProductsInHistoryTable(int idDate){
    this->ui->HistoryTable->clear();
    this->ui->HistoryTable->setRowCount(0);
    QSqlQuery* selectedDate = this->sql->getDateFromId(idDate);
    QString* timeArray = this->generateTimeArray(selectedDate);
    QSqlQuery* productsFromSelectedDateInHistory = this->sql->getProductsFromCurrentDate(timeArray);

    float priceMonth = 0;
    while(productsFromSelectedDateInHistory->next()){
        this->addRowInHistoryTable(productsFromSelectedDateInHistory->value("Name").toString(),productsFromSelectedDateInHistory->value("Koszt").toString()+" zł",
                     productsFromSelectedDateInHistory->value("Id").toString());
        priceMonth+=productsFromSelectedDateInHistory->value("Koszt").toFloat();
    }
    this->setPriceInHistory(priceMonth);

    delete timeArray;
    delete selectedDate;
}

QString* MainWindow::generateTimeArray(QSqlQuery* query){
    QString *array = new QString[3];
    array[0] = query->value(1).toString();
    array[1] = query->value(2).toString();
    array[2] = query->value(3).toString();
    return array;
}

void MainWindow::setPriceInHistory(float price){
    this->ui->priceHistory->setText(QString::number(price)+" zł");
}
