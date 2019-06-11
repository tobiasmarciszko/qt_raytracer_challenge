#include "imageitem.h"

ImageItem::ImageItem(QQuickItem *parent) : QQuickPaintedItem(parent)
{
}

void ImageItem::paint(QPainter *painter)
{
    painter->drawImage(0,0, m_image);
}

void ImageItem::setImage(const QImage &image)
{
    m_image = image;
    update();
}
