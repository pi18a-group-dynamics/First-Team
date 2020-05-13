#include "ingredients.h"
#include "ui_ingredients.h"
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>

Ingredients::Ingredients(QWidget *parent)
: QDialog(parent)
, ui_(new Ui::Ingredients) {
    ui_->setupUi(this);
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
    model_ = new QSqlQueryModel;
    model_->setQuery("SELECT id, name AS Название, meansurement AS Измерение FROM ingredients ORDER BY name;");
    ui_->ingr_table_->setModel(model_);
    ui_->name_ingr_line_->setValidator(new QRegExpValidator(QRegExp("[А-Я]{1}[a-я]{1,}")));
    ui_->mer_ingr_line_->setValidator(new QRegExpValidator(QRegExp("[А-Я]{1}[a-я]{1,}")));
    ui_->ingr_table_->setColumnHidden(0, true);
    ui_->ingr_table_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui_->ingr_table_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui_->ingr_table_->setSelectionMode(QTableView::SingleSelection);
    QObject::connect(ui_->ingr_table_, &QTableView::clicked, [this](const QModelIndex& index) {
        ui_->name_ingr_line_->setText(model_->data(model_->index(index.row(), 1)).toString());
        ui_->mer_ingr_line_->setText(model_->data(model_->index(index.row(), 2)).toString());
    });
}

void Ingredients::update_table() {
    model_->setQuery(model_->query().lastQuery());
    ui_->name_ingr_line_->setText("");
    ui_->mer_ingr_line_->setText("");
}

Ingredients::~Ingredients() {
    delete ui_;
}

void Ingredients::on_add_ingr_btn__clicked() {
    if (ui_->mer_ingr_line_->text().isEmpty() || ui_->name_ingr_line_->text().isEmpty()) {
        QMessageBox::warning(nullptr, "Нет введеных данных", "Введите данные");
        return;
    }
    QSqlQuery query;

    query.prepare("SELECT count(*) FROM ingredients WHERE name= :name, meansurement = :meansurement ");
    query.bindValue(":name", ui_->name_ingr_line_->text());
    query.bindValue(":meansurement", ui_->mer_ingr_line_->text());
    query.exec();
    query.first();
    if (!query.value(0).toBool()){
        QMessageBox::warning(nullptr, "Ингредиент уже есть", "Такой ингредиент уже существует");
        return;
    }

    query.prepare("INSERT INTO ingredients "
                  "VALUES(default,"
                  ":name,"
                  ":meansurement);");
    query.bindValue(":name", ui_->name_ingr_line_->text());
    query.bindValue(":meansurement", ui_->mer_ingr_line_->text());
    if (!query.exec()) {
        qDebug() << "error insert into ingredients";
        qDebug() << query.lastError().text();
        qDebug() << query.lastQuery();
    }
    update_table();
}

void Ingredients::on_erase_ingr_btn__clicked() {
    const QAbstractItemModel* model = ui_->ingr_table_->model();
    QString current_id = model->data(model->index(ui_->ingr_table_->currentIndex().row(), 0)).toString();
    if (ui_->mer_ingr_line_->text().isEmpty() || ui_->name_ingr_line_->text().isEmpty()) {
        QMessageBox::warning(nullptr, "Не выбран ингредиент", "Выберете ингредиент для удаления");
        return;
    }
    if (!QSqlQuery().exec("DELETE FROM ingredients WHERE id = " + current_id + ';')) {
        QMessageBox::warning(nullptr, "Не удалось удалить", "Запись не обнаружена");
        return;
    }
    update_table();
    QMessageBox::information(nullptr, "Удаление", "Удален ингредиент");
}

void Ingredients::on_change_ingr_btn__clicked() {
    const QAbstractItemModel* model = ui_->ingr_table_->model();
    QString current_id = model->data(model->index(ui_->ingr_table_->currentIndex().row(), 0)).toString();
    if (ui_->mer_ingr_line_->text().isEmpty() || ui_->name_ingr_line_->text().isEmpty()) {
        QMessageBox::warning(nullptr, "Не выбран ингредиент", "Выберете ингредиент для изменения");
        return;
    }
    QSqlQuery query;
    query.prepare("UPDATE ingredients SET "
                  "name = :name, "
                  "meansurement = :meansurement "
                  "WHERE id = :id;");
    query.bindValue(":name", ui_->name_ingr_line_->text());
    query.bindValue(":meansurement", ui_->mer_ingr_line_->text());
    query.bindValue(":id", current_id);
    if (!query.exec()) {
        QMessageBox::warning(nullptr, "Не удалось изменить", "Запись не обнаружена");
        return;
    }
    update_table();
    QMessageBox::information(nullptr, "Изменение", "Ингредиент изменён");
}
