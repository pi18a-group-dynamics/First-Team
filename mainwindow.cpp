#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QTableWidget>
#include <QPixmap>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui_(new Ui::MainWindow) {
    ui_->setupUi(this);
    connect_database();
    init_form();

}

void MainWindow::connect_database() {
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

QTableWidget* MainWindow::create_category_table(QString category_name) {
    QTableWidget* table = new QTableWidget;
    table->setColumnCount(3);
    table->setShowGrid(false);
    table->setColumnHidden(0, true);
    table->horizontalHeader()->hide();
    table->verticalHeader()->hide();
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QTableWidget::NoSelection);
    QObject::connect(table, &QTableWidget::clicked, [this, category_name](const QModelIndex& index) {
        const QAbstractItemModel* model = index.model();
        if (index.column() == 1) {
            QString recipies_id = model->data(model->index(index.row(), 0)).toString();
            QSqlQuery query;
            query.prepare("SELECT algorithm FROM recipies WHERE id = :id;");
            query.bindValue(":id", recipies_id);
            query.exec();
            query.next();
            ui_->algorithm_text_->setPlainText(query.value(0).toString());
            QSqlQueryModel* model = new QSqlQueryModel;
            model->setQuery("SELECT "
                            "i.name AS \"Ингредиент\", "
                            "CONCAT(CONCAT(CONCAT(ir.count, ' ('), i.meansurement), ')')"
                            " AS \"Количество\" FROM ingredients_of_recipies ir "
                            "LEFT JOIN ingredients i ON ir.ingredient_id = i.id "
                            "WHERE recipe_id = \'" + recipies_id + "\';");
            ui_->ingredients_table_->setModel(model);
            ui_->ingredients_table_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
            ui_->ingredients_table_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
            ui_->ingredients_table_->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
            ui_->ingredients_table_->setShowGrid(false);
        } else if (index.column() == 2) {
            QSqlQuery query;
            query.prepare("UPDATE recipies SET chosen = NOT :chosen WHERE id = :id;");
            query.bindValue(":chosen", model->data(model->index(index.row(), 2), Qt::UserRole).toBool());
            query.bindValue(":id", model->data(model->index(index.row(), 0)));
            query.exec();
            update_category(category_name);
        }
    });
    ui_->categories_tool_->addItem(table, category_name);
    return table;
}

void MainWindow::insert_recipies(QTableWidget* table, QSqlQuery& recipe) {
    table->setRowCount(table->rowCount() + 1);
    size_t row = table->rowCount() - 1;
    QTableWidgetItem* item = new QTableWidgetItem;
    if (recipe.value("chosen").toBool()) {
        item->setData(Qt::DecorationRole, QPixmap("/home/kirill/Загрузки/icons8-проверено-512.png").scaled(30, 30, Qt::KeepAspectRatio));
        item->setText("Избранный");
        item->setData(Qt::UserRole, recipe.value("chosen").toBool());
    } else {
        item->setData(Qt::DecorationRole, QPixmap("/home/kirill/Загрузки/icons8-отмена-512.png").scaled(30, 30, Qt::KeepAspectRatio));
        item->setText("Не избранное");
    }
    table->setRowHeight(row, 30);
    table->setItem(row, 2, item);
    item = new QTableWidgetItem;
    item->setText(recipe.value("name").toString());
    table->setItem(row, 1, item);
    item = new QTableWidgetItem;
    item->setText(recipe.value("id").toString());
    table->setItem(row, 0, item);
}

void MainWindow::update_category(QString category_name) { //Не работает.
    int category_index = 0;
    while (ui_->categories_tool_->itemText(category_index) != category_name) {
        if (ui_->categories_tool_->count() - 1 < category_index) { //Если не нашли нужную категорию
            ui_->categories_tool_->addItem(create_category_table(category_name), category_name);
            break;
        }
        ++category_index;
    }
    QTableWidget& table = dynamic_cast<QTableWidget&>(*(ui_->categories_tool_->widget(category_index)));
    table.clear();
    table.setRowCount(0);
    QSqlQuery recipies;
    recipies.prepare("SELECT * FROM recipies WHERE category_id = "
                     "(SELECT id FROM categories WHERE name = :category_name) ORDER BY chosen DESC, name;");
    recipies.bindValue(":category_name", category_name);
    if (!recipies.exec()) {
        QMessageBox::warning(this, "Предупреждение", "Не удалось обновить категорию: " + category_name);
        return;
    }
    while (recipies.next()) {
        insert_recipies(&table, recipies);
    }
}

void MainWindow::init_form() {
    ui_->categories_tool_->removeItem(0);
    QSqlQuery categories;
    categories.exec("SELECT * FROM categories;");
    QSqlQuery recipies;
    recipies.prepare("SELECT * FROM recipies WHERE category_id = :category_id ORDER BY chosen DESC, name;");
    QTableWidget* table;
    while (categories.next()) {
        table = create_category_table(categories.value("name").toString());
        recipies.bindValue(":category_id", categories.value("id"));
        recipies.exec();
        while (recipies.next()) {
            insert_recipies(table, recipies);
        }
    }
}

MainWindow::~MainWindow() {
    delete ui_;
    database_.close();
}
