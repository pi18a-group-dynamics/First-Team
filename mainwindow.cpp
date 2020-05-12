#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QTableWidget>
#include <QPixmap>

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
    QSqlQuery recipies;
    QTableWidget* table;
    auto new_table = []() -> QTableWidget* {        //Создаёт новую таблицу с настройками
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
            return table;
    };
    QTableWidgetItem* item;
    auto add_row = [&](size_t i) {                  //Вставляет строку в таблицу
        table->setRowCount(table->rowCount() + 1);
        item = new QTableWidgetItem;
        if (recipies.value("chosen").toBool()) {
            item->setData(Qt::DecorationRole, QPixmap("/home/kirill/Загрузки/icons8-проверено-512.png").scaled(30, 30, Qt::KeepAspectRatio));
            item->setText("Избранный");
        } else {
            item->setData(Qt::DecorationRole, QPixmap("/home/kirill/Загрузки/icons8-отмена-512.png").scaled(30, 30, Qt::KeepAspectRatio));
            item->setText("Не избранное");
        }
        table->setRowHeight(i, 30);
        table->setItem(i, 2, item);
        item = new QTableWidgetItem;
        item->setText(recipies.value("name").toString());
        table->setItem(i, 1, item);
        item = new QTableWidgetItem;
        item->setText(recipies.value("id").toString());
        table->setItem(i, 0, item);
    };
    while (categories.next()) {
        table = new_table();
        recipies.exec("SELECT * FROM recipies WHERE category_id = \'" + categories.value("id").toString() + "\' ORDER BY chosen DESC;");
        for (size_t i = 0; recipies.next(); ++i) {
            add_row(i);
        }
        ui_->categories_tool_->addItem(table, categories.value("name").toString());
    }
}

MainWindow::~MainWindow() {
    delete ui_;
    database_.close();
}

