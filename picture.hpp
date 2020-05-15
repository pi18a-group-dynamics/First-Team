#ifndef PICTURE_HPP
#define PICTURE_HPP

#include <QLabel>
#include <QByteArray>
#include <QPixmap>

class Picture : public QLabel {
    Q_OBJECT
public:
    explicit Picture(QWidget * = nullptr);
    explicit Picture(QPixmap);
    void set_pixmap(QPixmap);
    QPixmap& get_pixmap();
    static QByteArray to_bytea(const QPixmap&);
    static QPixmap from_bytea(const QByteArray&);
protected:
    void paintEvent(QPaintEvent*) override;
private:
    QPixmap pixmap_;
};

#endif // PICTURE_HPP
