#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFileDialog>
#include "recipe.hpp"
#include "ui_recipe.h"

Recipe::Recipe(OpenKey key, QWidget *parent)
: QWidget(parent)
, ui_(new Ui::Recipe) {
    ui_->setupUi(this);
    form_init();
    switch(key) {
        case OpenKey::push:
            push_init();
        break;
    }
}

void Recipe::form_init() {
    QTableWidget* table = ui_->ingredients_table_;
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels({"Ингредиент", "Количество"});
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    table->setSelectionMode(QTableWidget::SelectionMode::SingleSelection);
    table->setSelectionBehavior(QTableWidget::SelectRows);
    QSqlQuery categories;
    categories.exec("SELECT * FROM categories;");
    while (categories.next()) {
        ui_->categories_box_->addItem(categories.value("name").toString(), categories.value("id"));
    }
    QSqlQuery ingredients;
    ingredients.exec("SELECT * FROM ingredients;");
    while (ingredients.next()) {
        ui_->ingredient_combo_->addItem(ingredients.value("name").toString(), ingredients.value("id"));
    }
}

void Recipe::push_init() {
    connect(ui_->save_btn_, &QPushButton::clicked, [this]() {
       QSqlQuery query;
       QVariant recipe_id;
       query.prepare("INSERT INTO recipies VALUES(default, :category_id, :algorithm, :chosen, :name) RETURNING id;");
       query.bindValue(":category_id", ui_->categories_box_->currentData());
       query.bindValue(":algorithm", ui_->algorithm_text_->toPlainText());
       query.bindValue(":chosen", false);
       query.bindValue(":name", ui_->name_line_->text());
       query.exec();
       query.next();
       recipe_id = query.value(0);
       emit category_change(ui_->categories_box_->currentText());
       query.prepare("INSERT INTO recipe_photo VALUES(:id, :photo);");
       query.bindValue(":id", recipe_id);
       query.bindValue(":photo", Picture::to_bytea(ui_->photo_->get_pixmap()));
       query.exec();

       QTableWidget* table = ui_->ingredients_table_;
       const QAbstractItemModel* model = table->model();
       query.prepare("INSERT INTO ingredients_of_recipies VALUES(default, :ingredient_id, :recipe_id, :count);");
       query.bindValue(":recipe_id", recipe_id);
       for (int row = 0; row < table->rowCount(); ++row) {
           query.bindValue(":ingredient_id", model->data(model->index(row, 0), Qt::UserRole));
           query.bindValue(":count", model->data(model->index(row, 1), Qt::UserRole));
           query.exec();
       }
    });
}

Recipe::~Recipe() {
    delete ui_;
}

void Recipe::on_ingredient_combo__currentIndexChanged(int) {
    QSqlQuery meansurement;
    meansurement.prepare("SELECT meansurement FROM ingredients WHERE id = :id;");
    meansurement.bindValue(":id", ui_->ingredient_combo_->currentData());
    meansurement.exec();
    meansurement.next();
    ui_->meansurement_label_->setText(meansurement.value(0).toString());
}

void Recipe::on_photo_btn__clicked() {
    QString picture_path = QFileDialog::getOpenFileName(nullptr, "Загрузить фотографию рецепта", QString(), "*.png *.jpg *.ico *.icon");
    if (!picture_path.isEmpty()) {
        ui_->photo_->set_pixmap(picture_path);
    }
}

void Recipe::on_erase_photo_btn__clicked() {
    ui_->photo_->set_pixmap(QPixmap());
}

void Recipe::on_insert_btn__clicked() {
    QVariant ingredient_id = ui_->ingredient_combo_->currentData();
    QTableWidget* table = ui_->ingredients_table_;
    const QAbstractItemModel* model = table->model();
    int row = 0;
    for (; row < table->rowCount(); ++row) {
        if (model->data(model->index(row, 0), Qt::UserRole) == ingredient_id) {
            break;
        }
    }
    if (row == table->rowCount()) {
        table->setRowCount(row + 1);
    }
    QTableWidgetItem* item = new QTableWidgetItem;
    item->setText(ui_->ingredient_combo_->currentText());
    item->setData(Qt::UserRole, ui_->ingredient_combo_->currentData());
    table->setItem(row, 0, item);
    item = new QTableWidgetItem;
    item->setText(QString::number(ui_->ingredient_spin_->value()) + ' ' + ui_->meansurement_label_->text());
    item->setData(Qt::UserRole, ui_->ingredient_spin_->value());
    table->setItem(row, 1, item);
}

void Recipe::on_close_btn__clicked() {
    close();
}

void Recipe::on_ingredients_table__clicked(const QModelIndex &index) {
    QComboBox* box = ui_->ingredient_combo_;
    const QAbstractItemModel* model = index.model();
    box->setCurrentIndex(box->findData(model->data(model->index(index.row(), 0), Qt::UserRole)));
    ui_->ingredient_spin_->setValue(model->data(model->index(index.row(), 1), Qt::UserRole).toInt());
}
