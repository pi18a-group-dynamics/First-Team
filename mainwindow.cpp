#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QListView>
#include "ingredients.h"
#include "categories.h"
MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui_(new Ui::MainWindow) {
    ui_->setupUi(this);
    connect_database();
    init_form();
    QAction* action = new QAction;
    action->setText("Ингредиенты");
    connect(action, &QAction::triggered, [action]() {
        Ingredients* ingredients = new Ingredients;
        ingredients->setAttribute(Qt::WA_DeleteOnClose);
        action->setEnabled(false);
        ingredients->show();
        QObject::connect(ingredients, &Ingredients::destroyed, [action]() {
            action->setEnabled(true);
        });
    });
    ui_->menubar->addAction(action);
    QAction* action_2 = new QAction;
    action_2->setText("Категории");
    connect(action_2, &QAction::triggered, [action_2]() {
        Categories * categories = new  Categories;
        categories->setAttribute(Qt::WA_DeleteOnClose);
        action_2->setEnabled(false);
        categories->show();
        QObject::connect(categories, &Categories::destroyed, [action_2]() {
            action_2->setEnabled(true);
        });
    });
    ui_->menubar->addAction(action_2);

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

void MainWindow::init_form() {
    ui_->categories_tool_->removeItem(0);
    QSqlQuery categories;
    categories.exec("SELECT * FROM categories;");
    QSqlQueryModel* model;
    QListView* view;
    QSqlQuery recipies;
    while (categories.next()) {
        model = new QSqlQueryModel();
        model->setQuery("SELECT name FROM recipies WHERE category_id = \'" + categories.value("id").toString() + "\';");
        view = new QListView;
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setModel(model);
        ui_->categories_tool_->addItem(view, categories.value("name").toString());
    }
}

MainWindow::~MainWindow() {
    delete ui_;
    database_.close();
}

