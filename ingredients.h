#ifndef INGREDIENTS_H
#define INGREDIENTS_H

#include <QDialog>
#include <QMainWindow>
#include <QSqlDatabase>
namespace Ui {
class Ingredients;
}

class Ingredients : public QDialog
{
    Q_OBJECT

public:
    explicit Ingredients(QWidget *parent = nullptr);
     void connect_database();
     void form_init();
     void push_init();
    ~Ingredients();

private slots:
     void on_add_ingr_btn__clicked();

private:
    Ui::Ingredients *ui;
    QSqlDatabase database_;
};

#endif // INGREDIENTS_H
