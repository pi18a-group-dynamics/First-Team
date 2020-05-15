#ifndef CATEGORIES_H
#define CATEGORIES_H

#include <QWidget>

namespace Ui {
class Categories;
}

class Categories : public QWidget {
    Q_OBJECT
public:
    explicit Categories(QWidget *parent = nullptr);
    ~Categories();
private slots:
    void on_photo_btn__clicked();
    void on_close_btn__clicked();
    void on_push_btn__clicked();
    void on_categories_table__clicked(const QModelIndex &index);

    void on_erase_btn__clicked();

    void on_change_btn__clicked();

    void on_erase_photo_btn__clicked();

signals:
    void category_change(QString category_name);
    void all_update();
private:
    void form_init();
    void update_form();
    Ui::Categories *ui_;
};

#endif // CATEGORIES_H
