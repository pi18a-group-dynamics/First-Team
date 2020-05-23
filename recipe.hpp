#ifndef RECIPE_HPP
#define RECIPE_HPP

#include <QWidget>
#include <QModelIndex>

namespace Ui {
class Recipe;
}

class Recipe : public QWidget {
    Q_OBJECT
public:
    enum class OpenKey {push, change};
    explicit Recipe(OpenKey, QWidget *parent = nullptr);
    ~Recipe();
private:
    Ui::Recipe *ui_;
    void form_init();
    void push_init();
    void change_init();
signals:
    void category_change(QString category_name);
    void all_update();
private slots:
    void on_ingredient_combo__currentIndexChanged(int index);
    void on_photo_btn__clicked();
    void on_erase_photo_btn__clicked();
    void on_insert_btn__clicked();
    void on_close_btn__clicked();
    void on_ingredients_table__clicked(const QModelIndex &index);
};

#endif // RECIPE_HPP
