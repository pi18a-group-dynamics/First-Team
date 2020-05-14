#ifndef RECIPES_H
#define RECIPES_H

#include <QMainWindow>

namespace Ui {
class Recipes;
}

class Recipes : public QMainWindow
{
    Q_OBJECT

public:
    explicit Recipes(QWidget *parent = nullptr);
    ~Recipes();

private:
    Ui::Recipes *ui;
};

#endif // RECIPES_H
