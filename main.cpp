#include "mainwindow.h"
#include "ingredients.h"
#include <QApplication>
#include <iostream>
#include "categories.h"
int main(int argc, char *argv[]) try {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
} catch(const std::runtime_error& exception) {
    std::cerr << exception.what() << std::endl;
}
