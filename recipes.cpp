#include "recipes.h"
#include "ui_recipes.h"

Recipes::Recipes(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Recipes)
{
    ui->setupUi(this);
}

Recipes::~Recipes()
{
    delete ui;
}
