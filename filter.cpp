#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include "picture.hpp"
#include "filter.hpp"
#include "ui_filter.h"

Filter::Filter(QWidget *parent)
: QWidget(parent)
, ui_(new Ui::Filter) {
    ui_->setupUi(this);
    form_init();
}

void Filter::rows_move(QTableWidget *source, QTableWidget *destionation, QList<QTableWidgetItem*> items) {
    int row = 0;
    for (const auto& value : items) {
        row = value->row();
        int destionation_row = destionation->rowCount();
        destionation->setRowCount(destionation_row + 1);
        for (int i = 0; i < std::min(source->columnCount(), destionation->columnCount()); ++i) {
            destionation->setItem(destionation_row, i, source->item(row, i)->clone());
        }
        source->removeRow(row);
    }
    source->sortItems(0);
    destionation->sortItems(0);
}

void Filter::form_init() {
    ui_->right_category_->setColumnCount(1);
    ui_->right_category_->horizontalHeader()->setSelectionMode(QAbstractItemView::SingleSelection);
    ui_->right_category_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui_->right_category_->setHorizontalHeaderLabels({"Выбранные категории"});
    ui_->right_ingredients_->setColumnCount(1);
    ui_->right_ingredients_->horizontalHeader()->setSelectionMode(QAbstractItemView::SingleSelection);
    ui_->right_ingredients_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui_->right_ingredients_->setHorizontalHeaderLabels({"Выбранные ингредиенты"});
    //category init left table
    QSqlQuery query;
    query.exec("SELECT * FROM categories ORDER BY name;");
    QTableWidgetItem* item = nullptr;
    QTableWidget* table = ui_->left_category_;
    table->setColumnCount(1);
    table->horizontalHeader()->setSelectionMode(QAbstractItemView::SingleSelection);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setHorizontalHeaderLabels({"Доступные категории"});
    int row = 0;
    while (query.next()) {
        item = new QTableWidgetItem;
        item->setText(query.value("name").toString());
        item->setData(Qt::DecorationRole, Picture::from_bytea(query.value("photo").toByteArray()).scaled(50, 50, Qt::KeepAspectRatio));
        item->setData(Qt::UserRole, query.value("id"));
        row = table->rowCount();
        table->setRowCount(row + 1);
        table->setRowHeight(row,  50);
        table->setItem(row, 0, item);
    }
    //ingredients init left table
    query.exec("SELECT id, CONCAT(CONCAT(CONCAT(name, ' ('), meansurement), ')') AS name FROM ingredients ORDER BY name;");
    table = ui_->left_ingredients_;
    table->setColumnCount(1);
    table->horizontalHeader()->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setHorizontalHeaderLabels({"Доступные ингредиенты"});
    row = 0;
    while (query.next()) {
        item = new QTableWidgetItem;
        item->setText(query.value("name").toString());
        item->setData(Qt::UserRole, query.value("id"));
        row = table->rowCount();
        table->setRowCount(row + 1);
        table->setItem(row, 0, item);
    }
}

Filter::~Filter() {
    delete ui_;
}

void Filter::on_close_btn__clicked() {
    close();
}

void Filter::on_inser_category__clicked() {
    rows_move(ui_->left_category_, ui_->right_category_, ui_->left_category_->selectedItems());
}

void Filter::on_erase_category__clicked() {
    rows_move(ui_->right_category_, ui_->left_category_, ui_->right_category_->selectedItems());
}

void Filter::on_inser_ingredient__clicked() {
    rows_move(ui_->left_ingredients_, ui_->right_ingredients_, ui_->left_ingredients_->selectedItems());
}

void Filter::on_erase_ingredient__clicked() {
    rows_move(ui_->right_ingredients_, ui_->left_ingredients_, ui_->right_ingredients_->selectedItems());
}

void Filter::on_filter_btn__clicked() {
    QString query = "SELECT * FROM recipies r WHERE TRUE";
    if (QTableWidget* table = ui_->right_category_; table->rowCount()) {
        query += " AND category_id IN (";
        for (int i = 0; i < table->rowCount(); ++i) {
            query += table->item(i, 0)->data(Qt::UserRole).toString() + ',';
        }
        query.back() = ')';
    }
    if (QTableWidget* table = ui_->right_ingredients_; table->rowCount()) {
        query += " AND r.id IN (SELECT recipe_id FROM ingredients_of_recipies ir WHERE ir.ingredient_id IN (";
        for (int i = 0; i < table->rowCount(); ++i) {
            query += table->item(i, 0)->data(Qt::UserRole).toString() + ',';
        }
        query.back() = ')';
        query += ')';
    }
    if (QString name = ui_->name_line_->text(); !name.isEmpty()) {
        query += " AND name = \'" + name + '\'';
    }
    if (QString algorithm = ui_->algorithm_text_->toPlainText(); !algorithm.isEmpty()) {
        query += " AND algorithm = \'" + algorithm + '\'';
    }
    query += " AND category_id = :category_id ORDER BY r.name;";
    emit filter_change(std::move(query));
}

void Filter::on_default_btn__clicked() {
    emit default_filter();
}
