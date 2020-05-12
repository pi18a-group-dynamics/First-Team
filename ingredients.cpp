#include "ingredients.h"
#include "ui_ingredients.h"
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
Ingredients::Ingredients(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ingredients)
{
    ui->setupUi(this);
    connect_database();
    setWindowTitle("Ингредиенты");
    QSqlQueryModel * model;
    model = new QSqlQueryModel;
    model->setQuery("SELECT name, meansurement FROM ingredients;");
    model->setHeaderData(0, Qt::Orientation::Horizontal,"Название");
    model->setHeaderData(1, Qt::Orientation::Horizontal,"Измерение");
     ui->ingr_table_->setModel(model);
    ui->name_ingr_line_->setValidator(new QRegExpValidator(QRegExp("[А-Я]{1}[a-я]{1,}")));
    ui->mer_ingr_line_->setValidator(new QRegExpValidator(QRegExp("[А-Я]{1}[a-я]{1,}")));
}
void Ingredients::connect_database() {
    database_ = QSqlDatabase::addDatabase("QPSQL");
    database_.setPort(5432);
    database_.setHostName("localhost");
    database_.setUserName("postgres");
    database_.setPassword("postgres");
    database_.setDatabaseName("recipe_book");
    if (!database_.open()) {
        QMessageBox::critical(nullptr, "Ошибка подключения", "Ошибка соединения с базой данных");
        std::terminate();
    }
}

Ingredients::~Ingredients()
{
    delete ui;
}
