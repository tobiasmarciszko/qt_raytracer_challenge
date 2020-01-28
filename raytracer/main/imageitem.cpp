#include "imageitem.h"
#include <QQuickWindow>
#include <QtQuick/QSGFlatColorMaterial>
#include <QtQuick/QSGSimpleTextureNode>
#include <QSGNode>
#include <cmath>

ImageItem::ImageItem(QQuickItem *parent) : QQuickItem(parent)
{
    setFlag(QQuickItem::ItemHasContents, true);
}

QSGNode *ImageItem::updatePaintNode(
    QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *updatePaintNodeData) {
//
//    auto node = dynamic_cast<QSGSimpleTextureNode *>(oldNode);
//
//    if (!node) {
//        node = new QSGSimpleTextureNode();
//    }
//
//    QSGTexture *texture = window()->createTextureFromImage(m_image, QQuickWindow::TextureIsOpaque);
//    node->setOwnsTexture(true);
//    node->setRect(boundingRect());
//    node->markDirty(QSGNode::DirtyForceUpdate);
//    node->setTexture(texture);
//    return node;

    // QSGGeometryNode *node = nullptr;
    // QSGGeometry *geometry = nullptr;

    if (!oldNode) {
        oldNode = new QSGNode;
    }

    oldNode->removeAllChildNodes();

    for (const QLine& line: m_lines) {

        auto childNode = new QSGGeometryNode;

        auto geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 2);
        geometry->setLineWidth(2);
        geometry->setDrawingMode(QSGGeometry::DrawLineStrip);
        childNode->setGeometry(geometry);
        childNode->setFlag(QSGNode::OwnsGeometry);

        auto material = new QSGFlatColorMaterial;
        material->setColor(QColor(255, 0, 0));
        childNode->setMaterial(material);
        childNode->setFlag(QSGNode::OwnsMaterial);

        QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();

        vertices[0].set(line.x1(), line.y1());
        vertices[1].set(line.x2(), line.y2());

        oldNode->appendChildNode(childNode);
    }

    // oldNode->markDirty(QSGNode::DirtyGeometry);

    return oldNode;

}
