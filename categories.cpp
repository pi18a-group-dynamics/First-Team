#include "categories.h"
#include "ui_categories.h"

Categories::Categories(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Categories)
{
    ui->setupUi(this);
}

Categories::~Categories()
{
    delete ui;
}
