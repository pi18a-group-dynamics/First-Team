#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QListView>

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

