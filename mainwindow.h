#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QDate>

#include "sqlmanager.h"
#include "ui_mainwindow.h"
#include "addbox.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    QString getCurrentDate();
    void setCurrentDate(QString newCurrentDate);
    void checkCurrentDate();

    void setIdCurrentDate(QString IdCurrentDate);
    QString getIdCurrentDate();

    void setProductsCurrentDate(QSqlQuery *IdCurrentDate);
    QSqlQuery *getProductsCurrentDate();


    void setCurrentYear(QString currentYear);
    QString getCurrentYear();

    void setCurrentMonth(QString currentMonth);
    QString getCurrentMonth();

    QString* getTimeArray();

    void setRowToTodayTable(QString productName, float price, int id);

    int getSelectedRow();

    int getIdSqlSelectedCostRow();

    void setTitleLabel();

    void insertMounthInHistoryTab();

    ~MainWindow();

private slots:
    void on_AddButton_clicked();
    void slotAddedNewProduct();
    void slotAddedExistProduct();

    void on_DeleteRow_clicked();

    void on_TodayTable_clicked(const QModelIndex &index);

    void on_MounthHistory_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    AddBox *dialog;
    SqlManager * sql;
    QString currentDate;
    QString currentYear;
    QString currentMonth;
    QString *timeArray;
    QString idCurrentDate;
    QString* generateTimeArray(QSqlQuery* query);
    QSqlQuery *productsCurrentDate;
    QSqlQuery *productsInHistory;
    QSqlQuery *products;
    float priceInCurrentDate;
    int selectedRow;
    int idSqlSelectedCostRow;

    void setTimeArray();
    void prepareDb();
    void prepareTodayTable();
    void prepareHistoryTable();
    void getProductsWithCurrentDate();
    void insertProductsInTodayTable();
    void insertProductsInHistoryTable(int idDate);
    void setPriceInMonth(float price);
    void prepareAddBox();
    void AddBox_addProductsToComboBox();
    void addRowInTodayTable(QString name, QString price, QString Id);
    void addRowInHistoryTable(QString name, QString price, QString Id);
    void setPriceInCurrentDate(float price);
    void setPriceInHistory(float price);
    float getPriceInCurrentDate();
    void setSelectedRow(int row);
    void setIdSqlSelectedCostRow(int id);

};

#endif // MAINWINDOW_H
