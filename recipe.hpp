#ifndef RECIPE_HPP
#define RECIPE_HPP

#include <QWidget>

namespace Ui {
class Recipe;
}

class Recipe : public QWidget {
    Q_OBJECT
public:
    explicit Recipe(QWidget *parent = nullptr);
    ~Recipe();
private:
    Ui::Recipe *ui;
signals:
    void category_change(QString category_name);
    void all_update();
};

#endif // RECIPE_HPP
