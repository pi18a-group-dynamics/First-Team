#ifndef FILTER_HPP
#define FILTER_HPP

#include <QWidget>
#include <QTableWidget>

namespace Ui {
class Filter;
}

class Filter : public QWidget {
    Q_OBJECT
public:
    explicit Filter(QWidget *parent = nullptr);
    ~Filter();
private slots:
    void on_close_btn__clicked();
    void on_inser_category__clicked();
    void on_erase_category__clicked();
    void on_inser_ingredient__clicked();
    void on_erase_ingredient__clicked();
    void on_filter_btn__clicked();
signals:
    void filter_change(QString);
private:
    Ui::Filter *ui_;
    void form_init();
    void rows_move(QTableWidget* source, QTableWidget* destionation, QList<QTableWidgetItem*> items);
};

#endif // FILTER_HPP
