#ifndef INGREDIENTS_H
#define INGREDIENTS_H

#include <QDialog>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>
namespace Ui {
class Ingredients;
}

class Ingredients : public QDialog {
    Q_OBJECT
public:
    explicit Ingredients(QWidget *parent = nullptr);
    void form_init();
    void update_table();
    ~Ingredients();
private slots:
    void on_add_ingr_btn__clicked();
    void on_erase_ingr_btn__clicked();
    void on_change_ingr_btn__clicked();
    void on_close_btn__clicked();
signals:
    void category_change(QString category_name);
    void all_update();
private:
    Ui::Ingredients* ui_;
    QSqlDatabase database_;
    QSqlQueryModel* model_;
};

#endif // INGREDIENTS_H
