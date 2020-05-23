#include "recipe.hpp"
#include "ui_recipe.h"

Recipe::Recipe(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Recipe)
{
    ui->setupUi(this);
}

Recipe::~Recipe()
{
    delete ui;
}
