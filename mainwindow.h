#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    void init_form();
    void connect_database();
    ~MainWindow();
private:
    Ui::MainWindow *ui_;
    QSqlDatabase database_;
};
#endif // MAINWINDOW_H
