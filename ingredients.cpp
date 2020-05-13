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
    model = new QSqlQueryModel;
    model->setQuery("SELECT id,name AS Название, meansurement AS Измерение FROM ingredients;");
    ui->ingr_table_->setModel(model);
    ui->name_ingr_line_->setValidator(new QRegExpValidator(QRegExp("[А-Я]{1}[a-я]{1,}")));
    ui->mer_ingr_line_->setValidator(new QRegExpValidator(QRegExp("[А-Я]{1}[a-я]{1,}")));
    ui->ingr_table_->setColumnHidden(0, true);
    QObject::connect(ui->ingr_table_,&QTableView::clicked,[this](const QModelIndex & index){
        ui->name_ingr_line_->setText(model->data(model->index(index.row(),1)).toString());
        ui->mer_ingr_line_->setText(model->data(model->index(index.row(),2)).toString());
        current_id= model->data(model->index(index.row(),0)).toString();
    });
}

void Ingredients::update_table(){
    model->setQuery(model->query().lastQuery());
    ui->name_ingr_line_->setText("");
    ui->mer_ingr_line_->setText("");
}

Ingredients::~Ingredients()
{
    delete ui;
}

void Ingredients::on_add_ingr_btn__clicked()
{
    if(ui->mer_ingr_line_->text().isEmpty() || ui->name_ingr_line_->text().isEmpty()){
        QMessageBox::warning(nullptr,"Нет введеных данных", "Введите данные");
    } else {
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
    update_table();
    }
}

void Ingredients::on_erase_ingr_btn__clicked()
{
    if(ui->mer_ingr_line_->text().isEmpty() || ui->name_ingr_line_->text().isEmpty()){
        QMessageBox::warning(nullptr,"Не выбран ингредиент", "Выберете ингредиент для удаления");
    } else {

        QSqlQuery query;
        if(query.exec("SELECT id FROM ingredients WHERE id="+current_id+";")&&  query.last()==true){

        if(query.exec("DELETE FROM ingredients WHERE id="+current_id+";")){
            update_table();
             QMessageBox::information(nullptr,"Удаление", "Удален ингредиент");
        } else {
             QMessageBox::information(nullptr,"Удаление", "Не удалось удалить");
        }
    } else {
           QMessageBox::information(nullptr,"Удаление", "Запись не обнаружена");
        }

    }
}

void Ingredients::on_change_ingr_btn__clicked()
{
    if(ui->mer_ingr_line_->text().isEmpty() || ui->name_ingr_line_->text().isEmpty()){
        QMessageBox::warning(nullptr,"Не выбран ингредиент", "Выберете ингредиент для изменения");
    } else {

        QSqlQuery query;
        if(query.exec("SELECT id FROM ingredients WHERE id="+current_id+";")&&  query.last()==true){

        if(query.exec("UPDATE ingredients SET name= '"+ui->name_ingr_line_->text()+
                      "', meansurement = '"+ui->mer_ingr_line_->text()+"'"
                      """WHERE id="+current_id+";")){
            update_table();
             QMessageBox::information(nullptr,"Изменение", "Изменен ингредиент");
        } else {
             QMessageBox::information(nullptr,"Изменениее", "Не удалось изменить");
        }
    } else {
           QMessageBox::information(nullptr,"Изменение", "Запись не обнаружена");
        }

    }
}
