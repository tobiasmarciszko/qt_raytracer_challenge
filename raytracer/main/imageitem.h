#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include <QImage>
#include <QPainter>
#include <QQuickItem>
#include <QtQuick/QSGTexture>
#include <QQuickWindow>

class ImageItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QImage image READ getImage WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(QList<QLine> lines READ getLines WRITE setLines NOTIFY linesChanged)

public:
    explicit ImageItem(QQuickItem *parent = nullptr);

    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData) override;

signals:
    void imageChanged();
    void linesChanged();

private:

    void setLines(const QList<QLine>& lines) {
        m_lines = lines;
        update();
    }

    void setImage(const QImage& image) {
        m_image = image;
        count = count + 1;
        update();
        // emit imageChanged();
    }

    QImage getImage() {
        return m_image;
    }

    QList<QLine> getLines() {
        return m_lines;
    }

    QImage m_image;
    float count{0};
    QList<QLine> m_lines{};
};
#endif // IMAGEITEM_H
