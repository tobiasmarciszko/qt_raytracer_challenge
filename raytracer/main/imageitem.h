#ifndef IMAGEITEM_H
#define IMAGEITEM_H
#include <QQuickPaintedItem>
#include <QQuickItem>
#include <QPainter>
#include <QImage>

class ImageItem : public QQuickPaintedItem
{
    Q_OBJECT
public:
    explicit ImageItem(QQuickItem *parent = nullptr);
    Q_INVOKABLE void setImage(const QImage &image);
    void paint(QPainter *painter) override;

private:
    QImage m_image;
};
#endif // IMAGEITEM_H
