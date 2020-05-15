#include "picture.hpp"
#include <QPaintEvent>
#include <QBuffer>
#include <QPainter>

Picture::Picture(QWidget* parent)
: QLabel(parent) {

}

Picture::Picture(QPixmap pixmap)
: pixmap_(pixmap) {
    update();
}

void Picture::set_pixmap(QPixmap pixmap) {
    pixmap_ = pixmap;
    update();
}

void Picture::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QSize current_size_picture = (size().width() > pixmap_.width() && size().height() > pixmap_.height()) //Если размер виджета по ширине и высоте больше чем размер исходной картинки
                                 ? pixmap_.size()                                                         //То устанавливаем размер картинки (не хочется терять качество)
                                 : size();                                                                //Если размер меньше, то при помощи гопотетии уменьшаем картинку
    QPixmap current_pixmap = pixmap_.scaled(current_size_picture, Qt::AspectRatioMode::KeepAspectRatio);
    QPoint pixmap_point((size().width() - current_pixmap.width())/2, (size().height() - current_pixmap.height())/2);
    painter.drawPixmap(pixmap_point, current_pixmap);
    QLabel::paintEvent(event);
}

QByteArray Picture::to_bytea(const QPixmap &pixmap) {
    QByteArray bytea;
    QBuffer buffer(&bytea);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");
    return bytea; //NRVO
}

QPixmap Picture::from_bytea(const QByteArray &bytea) {
    QPixmap pixmap;
    pixmap.loadFromData(bytea, "PNG");
    return pixmap;
}
