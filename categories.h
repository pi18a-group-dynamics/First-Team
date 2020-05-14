#ifndef CATEGORIES_H
#define CATEGORIES_H

#include <QWidget>

namespace Ui {
class Categories;
}

class Categories : public QWidget
{
    Q_OBJECT

public:
    explicit Categories(QWidget *parent = nullptr);
    void form_init();
    ~Categories();

private slots:
    void on_close_btn__clicked();

private:
    Ui::Categories *ui_;
};

#endif // CATEGORIES_H
