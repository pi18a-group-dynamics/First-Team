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
#include <QListView>
#include <type_traits>
#include "filter.hpp"
#include "ingredients.h"
#include "categories.h"
#include "recipe.hpp"

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

QTableWidget* MainWindow::create_category_table(QString category_name, QPixmap photo) {
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
                            "CONCAT(CONCAT(ir.count, ' '), i.meansurement) "
                            "AS \"Количество\" FROM ingredients_of_recipies ir "
                            "LEFT JOIN ingredients i ON ir.ingredient_id = i.id "
                            "WHERE recipe_id = \'" + recipies_id + "\';");
            ui_->ingredients_table_->setModel(model);
            ui_->ingredients_table_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
            ui_->ingredients_table_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
            ui_->ingredients_table_->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
            ui_->ingredients_table_->setShowGrid(false);
            query.prepare("SELECT photo FROM recipe_photo WHERE id = :id");
            query.bindValue(":id", recipies_id);
            query.exec();
            if (query.next()) {
                ui_->photo_->set_pixmap(Picture::from_bytea(query.value("photo").toByteArray()));
            }
        } else if (index.column() == 2) {
            QSqlQuery query;
            query.prepare("UPDATE recipies SET chosen = NOT :chosen WHERE id = :id;");
            query.bindValue(":chosen", model->data(model->index(index.row(), 2), Qt::UserRole).toBool());
            query.bindValue(":id", model->data(model->index(index.row(), 0)));
            query.exec();
            update_category(category_name);
        }
    });
    ui_->categories_tool_->addItem(table, photo, category_name);
    return table;
}

void MainWindow::insert_recipies(QTableWidget* table, const Recipe& recipe) {
    table->setRowCount(table->rowCount() + 1);
    size_t row = table->rowCount() - 1;
    QTableWidgetItem* item = new QTableWidgetItem;
    if (recipe.chosen) {
        item->setData(Qt::DecorationRole, QPixmap("/home/kirill/Загрузки/icons8-проверено-512.png").scaled(30, 30, Qt::KeepAspectRatio));
        item->setText("Избранный");
    } else {
        item->setData(Qt::DecorationRole, QPixmap("/home/kirill/Загрузки/icons8-отмена-512.png").scaled(30, 30, Qt::KeepAspectRatio));
        item->setText("Не избранное");
    }
    item->setData(Qt::UserRole, recipe.chosen);
    table->setRowHeight(row, 30);
    table->setItem(row, 2, item);
    item = new QTableWidgetItem;
    item->setText(recipe.name);
    table->setItem(row, 1, item);
    item = new QTableWidgetItem;
    item->setText(recipe.id);
    table->setItem(row, 0, item);
}

void MainWindow::insert_recipies(QTableWidget* table, QSqlQuery query) {
    Recipe recipe;
    recipe.id          = query.value("id").toString();
    recipe.category_id = query.value("category_id").toString();
    recipe.algorithm   = query.value("algorithm").toString();
    recipe.chosen      = query.value("chosen").toBool();
    recipe.name        = query.value("name").toString();
    insert_recipies(table, recipe);
}

void MainWindow::update_category(QString category_name) {
    int category_index = 0;
    QSqlQuery query;
    query.prepare("SELECT photo FROM categories WHERE name = :name;");
    query.bindValue(":name", category_name);
    QPixmap photo;
    if (query.exec() && query.next()) {
        photo = Picture::from_bytea(query.value("photo").toByteArray());
    }
    while (ui_->categories_tool_->itemText(category_index) != category_name) {
        if (ui_->categories_tool_->count() - 1 < category_index) { //Если не нашли нужную категорию
            create_category_table(category_name, photo);
            break;
        }
        ++category_index;
    }
    QTableWidget& table = dynamic_cast<QTableWidget&>(*(ui_->categories_tool_->widget(category_index)));
    ui_->categories_tool_->setItemIcon(category_index, photo);
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
    ui_->photo_->set_pixmap(QPixmap());
    ui_->algorithm_text_->clear();
    ui_->ingredients_table_->setModel(nullptr);
}

void MainWindow::update_form() {
    QToolBox* tool = ui_->categories_tool_;
    while (tool->count()) {
        tool->removeItem(0);
    }
    QSqlQuery categories;
    categories.exec("SELECT * FROM categories ORDER BY name;");
    QSqlQuery recipies;
    recipies.prepare("SELECT * FROM recipies WHERE category_id = :category_id ORDER BY chosen DESC, name;");
    QTableWidget* table;
    QPixmap photo;
    while (categories.next()) {
        photo = Picture::from_bytea(categories.value("photo").toByteArray());
        table = create_category_table(categories.value("name").toString(), photo);
        recipies.bindValue(":category_id", categories.value("id"));
        recipies.exec();
        while (recipies.next()) {
            insert_recipies(table, recipies);
        }
    }
}

void MainWindow::init_form() {
    update_form();
    auto create_action = [this](auto form, QString name, auto&&... args) {
        QAction* action = new QAction;
        action->setText(name);
        connect(action, &QAction::triggered, [this, action, form, args...]() mutable {
            form = new std::remove_pointer_t<decltype(form)>(args...);
            form->setAttribute(Qt::WA_DeleteOnClose);
            QObject::connect(form, &std::remove_pointer_t<decltype(form)>::category_change, this, &MainWindow::update_category);
            QObject::connect(form, &std::remove_pointer_t<decltype(form)>::all_update, this, &MainWindow::update_form);
            action->setEnabled(false);
            form->show();
            QObject::connect(form, &Ingredients::destroyed, [action]() {
                action->setEnabled(true);
            });
        });
        ui_->menubar->addAction(action);
    };
    create_action(static_cast<::Recipe*>(nullptr), "Рецепты", ::Recipe::OpenKey::push);
    create_action(static_cast<Categories*>(nullptr), "Категории");
    create_action(static_cast<Ingredients*>(nullptr), "Ингридиенты");
}

MainWindow::~MainWindow() {
    delete ui_;
    database_.close();
}

void MainWindow::on_erase_btn__clicked() {
    QMessageBox question;
    question.setWindowTitle("Подтвердите удаление");
    question.setText("Удалить рецепт?");
    question.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    question.setButtonText(QMessageBox::Yes, "Удалить");
    question.setButtonText(QMessageBox::No, "Не удалять");
    if (question.exec() == QMessageBox::No) {
        return;
    }
    QModelIndex index = dynamic_cast<QTableWidget*>(ui_->categories_tool_->currentWidget())->currentIndex();
    const QAbstractItemModel* model = index.model();
    QString recipe_id = model->data(model->index(index.row(), 0)).toString();
    QSqlQuery query;
    query.prepare("SELECT c.name FROM recipies r JOIN categories c ON r.category_id = c.id WHERE r.id = :id;");
    query.bindValue(":id", recipe_id);
    query.exec();
    query.next();
    QString category_name = query.value(0).toString();
    query.prepare("DELETE FROM recipies WHERE id = :id;");
    query.bindValue(":id", recipe_id);
    query.exec();
    update_category(category_name);
}

void MainWindow::on_open_btn__clicked() {
    QTableWidget* table = dynamic_cast<QTableWidget*>(ui_->categories_tool_->currentWidget());
    if (table->currentRow() == -1) {
        QMessageBox::warning(nullptr, "Ошибка", "Выберите рецепт");
        return;
    }
    QAbstractItemModel* model = table->model();
    QString recipies_id = model->data(model->index(table->currentRow(), 0)).toString();
    ::Recipe* recipe = new ::Recipe(::Recipe::OpenKey::change, recipies_id);
    recipe->setAttribute(Qt::WA_DeleteOnClose);
    connect(recipe, &QWidget::destroyed, [this]() {
        ui_->open_btn_->setEnabled(true);
    });
    connect(recipe, &::Recipe::category_change, this, &MainWindow::update_category);
    ui_->open_btn_->setEnabled(false);
    recipe->show();
}

void MainWindow::set_filter(QString query) {
    while (ui_->categories_tool_->count()) {
        ui_->categories_tool_->removeItem(0);
    }
    QSqlQuery categories;
    categories.exec("SELECT * FROM categories ORDER BY name;");
    QSqlQuery recipies;
    recipies.prepare(query);
    QTableWidget* table;
    QPixmap photo;
    while (categories.next()) {
        recipies.bindValue(":category_id", categories.value("id"));
        recipies.exec();
        qDebug() << recipies.lastQuery();
        qDebug() << categories.value("id").toString();
        qDebug() << recipies.size();
        if (!recipies.size()) {
            continue;
        }
        photo = Picture::from_bytea(categories.value("photo").toByteArray());
        table = create_category_table(categories.value("name").toString(), photo);
        while (recipies.next()) {
            insert_recipies(table, recipies);
        }
    }
}

void MainWindow::on_filter_btn__clicked() {
    Filter* filter = new Filter;
    filter->setAttribute(Qt::WA_DeleteOnClose);
    connect(filter, &Filter::filter_change, this, &MainWindow::set_filter);
    ui_->filter_btn_->setEnabled(false);
    connect(filter, &QWidget::destroyed, [this]() {ui_->filter_btn_->setEnabled(true); });
    filter->show();
}


