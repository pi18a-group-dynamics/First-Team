#include "categories.h"
#include "ui_categories.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>

Categories::Categories(QWidget *parent) :
    QWidget(parent),
    ui_(new Ui::Categories) {
    ui_->setupUi(this);
    form_init();
}

void Categories::form_init(){
     setWindowTitle("Категории");
        ui_->categ_table_->setColumnCount(3);
        ui_->categ_table_->setColumnHidden(0, true);
        ui_->categ_table_->setHorizontalHeaderLabels({" ","Название","Эмблема"});
        QSqlQuery query;
        query.exec("SELECT * FROM categories ORDER BY name;");
       for (int i=0; query.next(); ++i){
           ui_->categ_table_->insertRow(i);
           ui_->categ_table_->setItem(i,0, new QTableWidgetItem(query.value(0).toString()));
           ui_->categ_table_->setItem(i,1, new QTableWidgetItem(query.value(1).toString()));
           ui_->categ_table_->setItem(i,2, new QTableWidgetItem(query.value(2).toString()));
       }

       QObject::connect(ui_->categ_table_, &QTableWidget::clicked, [this](const QModelIndex& index) {
         const QAbstractItemModel* model = index.model();
         ui_->name_line_->setText(model->data(model->index(index.row(),1)).toString());
       });

}

Categories::~Categories()
{
    delete ui_;
}


void Categories::on_close_btn__clicked()
{
    close();
}
