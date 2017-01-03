#include <QGraphicsItem>

#ifndef PIONITEM_H
#define PIONITEM_H

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;
QT_END_NAMESPACE

class pionitem : public QGraphicsItem
{
public:
    pionitem();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

#endif // PIONITEM_H
