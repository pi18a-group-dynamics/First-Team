#include "mainwindow.h"
#include "ingredients.h"
#include <QApplication>
#include <iostream>
<<<<<<< HEAD
#include <QByteArray>
#include <QBuffer>
#include <QPixmap>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>

int main(int argc, char *argv[]) try {
      QApplication a(argc, argv);
    MainWindow w;
=======
#include "categories.h"
int main(int argc, char *argv[]) try {
    QApplication a(argc, argv);
    Categories w;

>>>>>>> categories_form

    w.show();

//    QSqlDatabase database_;
//    database_ = QSqlDatabase::addDatabase("QPSQL");
//    database_.setPort(5432);
//    database_.setHostName("localhost");
//    database_.setUserName("postgres");
//    database_.setPassword("postgres");
//    database_.setDatabaseName("recipe_book");
//    if (!database_.open()) {
//        std::terminate();
//    }
//    QSqlQuery query;
//    QPixmap photo{"/home/kirill/Загрузки/Торт.png"};
//    QByteArray bytea;
//    QBuffer buffer;
//    buffer.setBuffer(&bytea);
//    buffer.open(QIODevice::WriteOnly);
//    photo.save(&buffer, "PNG");
//    query.prepare("update categories set photo = :photo WHERE id = :id;");
//    query.bindValue(":photo", bytea);
//    query.bindValue(":id", 8);
//    if (!query.exec()) {
//        qDebug() << query.lastQuery();
//        qDebug() << query.lastError().text();
//    }
    return a.exec();
} catch(const std::runtime_error& exception) {
    std::cerr << exception.what() << std::endl;
}
