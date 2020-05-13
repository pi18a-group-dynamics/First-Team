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

QTableWidget* MainWindow::new_table_category() {
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
    QObject::connect(table, &QTableWidget::clicked, [this](const QModelIndex& index) {
        const QAbstractItemModel* model = index.model();
        if (index.column() == 1) {
            qDebug() << "Название: " << model->data(model->index(index.row(), 1)).toString();
        } else if (index.column() == 2) {
            qDebug() << "Избранность: " <<  model->data(model->index(index.row(), 2)).toString();
        }
    });
    return table;
}

void MainWindow::update_category(QString category_name) {
    int category_index = 0;
    while (ui_->categories_tool_->itemText(category_index++) != category_name) {
        if (ui_->categories_tool_->count() - 1 < category_index) { //Если не нашли нужную категорию
            ui_->categories_tool_->addItem(new QTableWidget, category_name);
            break;
        }
    }
    QTableWidget& table = dynamic_cast<QTableWidget&>(*(ui_->categories_tool_->widget(category_index)));
    table.clear();
    QSqlQuery recipies;
    recipies.prepare("SELECT * FROM recipies WHERE category_id = "
                     "(SELECT id FROM categories WHERE name = :category_name) ORDER BY chosen DESC;");
    recipies.bindValue(":category_name", category_name);
    if (recipies.exec()) {
        QMessageBox::warning(this, "Предупреждение", "Не удалось обновить категорию: " + category_name);
        return;
    }
    while (recipies.next()) {
        table.setRowCount(table.rowCount() + 1);
    }
}

void MainWindow::init_form() {
    ui_->categories_tool_->removeItem(0);
    QSqlQuery categories;
    categories.exec("SELECT * FROM categories;");
    QSqlQuery recipies;
    recipies.prepare("SELECT * FROM recipies WHERE category_id = :category_id ORDER BY chosen DESC;");
    QTableWidget* table;
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
        table = new_table_category();
        recipies.bindValue(":category_id", categories.value("id"));
        recipies.exec();
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

