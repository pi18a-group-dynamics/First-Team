#include "categories.h"
#include "ui_categories.h"
#include <QFileDialog>
#include <QByteArray>
#include <QMessageBox>
#include <QBuffer>
#include <QSqlQuery>

Categories::Categories(QWidget *parent)
:   QWidget(parent)
,   ui_(new Ui::Categories) {
    ui_->setupUi(this);
    form_init();
}

Categories::~Categories() {
    delete ui_;
}

void Categories::form_init() {
    QTableWidget* table = ui_->categories_table_;
    table->setColumnCount(1);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    table->setHorizontalHeaderLabels({"Категория"});
    update_form();
}

void Categories::update_form() {
    ui_->photo_->set_pixmap(QPixmap()); //set_pixmap(null)
    ui_->name_line_->clear();
    QSqlQuery query;
    query.exec("SELECT * FROM categories ORDER BY name;");
    QTableWidget* table = ui_->categories_table_;
    table->clear();
    table->setRowCount(0);
    QTableWidgetItem* item;
    QPixmap photo;
    size_t i;
    while (query.next()) {
        i = table->rowCount();
        table->setRowCount(i + 1);
        item = new QTableWidgetItem;
        item->setData(Qt::UserRole, query.value("id"));
        item->setText(query.value("name").toString());
        photo.loadFromData(query.value("photo").toByteArray(), "PNG");
        if (!photo.isNull()) {
            item->setData(Qt::DecorationRole, photo.scaled(30, 30, Qt::KeepAspectRatio));
        }
        table->setItem(i, 0, item);
        table->setRowHeight(i, 30);
    }
}

void Categories::on_photo_btn__clicked() {
    QString picture_path = QFileDialog::getOpenFileName(nullptr, "Загрузить фотографию категории", QString(), "*.png *.jpg *.ico *.icon");
    if (!picture_path.isEmpty()) {
        ui_->photo_->set_pixmap(picture_path);
    }
}

void Categories::on_close_btn__clicked() {
    close();
}

void Categories::on_push_btn__clicked() {
    QSqlQuery query;
    query.prepare("INSERT INTO categories VALUES("
                  "default,"
                  ":name,"
                  ":photo);");
    query.bindValue(":name", ui_->name_line_->text());
    QByteArray bytea;
    QBuffer buffer(&bytea);
    buffer.open(QIODevice::WriteOnly);
    ui_->photo_->get_pixmap().save(&buffer, "PNG");
    query.bindValue(":photo", bytea);
    query.exec() ? QMessageBox::information(nullptr, "Успех", "Создана новая категория")
                 : QMessageBox::warning(nullptr, "Ошибка", "Такая категория уже существует");
    update_form();
}

void Categories::on_categories_table__clicked(const QModelIndex &index) {
    const QAbstractItemModel* model = index.model();
    ui_->name_line_->setText(model->data(index).toString());
    QSqlQuery query;
    query.prepare("SELECT photo FROM categories WHERE id = :id;");
    query.bindValue(":id", model->data(index, Qt::UserRole).toString());
    query.exec();
    if (query.next()) {
        QByteArray bytea{query.value("photo").toByteArray()};
        QPixmap photo;
        photo.loadFromData(bytea, "PNG");
        ui_->photo_->set_pixmap(std::move(photo));
    }
}

void Categories::on_erase_btn__clicked() {
    QTableWidget* table{ui_->categories_table_};
    int current_index{table->currentRow()};
    if (current_index == -1) {
        QMessageBox::warning(nullptr, "Ошибка", "Выберите категорию для удаления");
        return;
    }
    QString category_id = table->item(current_index, 0)->data(Qt::UserRole).toString();
    QSqlQuery query;
    query.prepare("DELETE FROM categories WHERE id = :id;");
    query.bindValue(":id", category_id);
    query.exec() ? update_form(), QMessageBox::information(nullptr, "Успех", "Категория успешно удалена")
                 : QMessageBox::warning(nullptr, "Ошибка", "Ошибка удаления");
}