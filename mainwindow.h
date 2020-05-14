#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    struct Recipe;
    MainWindow(QWidget *parent = nullptr);
    void init_form();
    void connect_database();
    ~MainWindow();
private:
    Ui::MainWindow *ui_;
    QSqlDatabase database_;
    QTableWidget* create_category_table(QString category_name);         //Возвращает настроенную таблицу для категории
    void insert_recipies(QTableWidget* table, const Recipe& recipe);         //Вставка рецепта в категорию
    void insert_recipies(QTableWidget* table, QSqlQuery query);
private slots:
    void update_category(QString category_name);        //Обновлнение одной категории (одного пункта)
};

struct MainWindow::Recipe {
    QString id;
    QString category_id;
    QString algorithm;
    bool chosen;
    QString name;
};

#endif // MAINWINDOW_H
