#ifndef PICTURE_HPP
#define PICTURE_HPP

#include <QLabel>
#include <QPixmap>

class Picture : public QLabel {
    Q_OBJECT
public:
    Picture();
    explicit Picture(QWidget * = nullptr);
    explicit Picture(QPixmap);
    void set_pixmap(QPixmap);
protected:
    void paintEvent(QPaintEvent*) override;
private:
    QPixmap pixmap_;
};

#endif // PICTURE_HPP
