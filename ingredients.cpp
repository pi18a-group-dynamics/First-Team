#include "ingredients.h"
#include "ui_ingredients.h"
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
Ingredients::Ingredients(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ingredients)
{
    ui->setupUi(this);
    connect_database();
    form_init();
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
void Ingredients::form_init() {
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

void Ingredients::push_init(){
  QSqlQuery query;
  query.prepare("INSERT INTO ingredients "
                "VALUES(default,"
                ":name,"
                ":meansurement);");
  query.bindValue(":name",ui->name_ingr_line_->text());
  query.bindValue(":meansurement",ui->mer_ingr_line_->text());
  if (!query.exec()) {
          qDebug() << "error insert into " << "ingredients";
          qDebug() << query.lastError().text();
          qDebug() << query.lastQuery();
      }

}
Ingredients::~Ingredients()
{
    delete ui;
}

void Ingredients::on_add_ingr_btn__clicked()
{
    //нужно добавить проверку на пустоту полей
    push_init();
    form_init();

}
