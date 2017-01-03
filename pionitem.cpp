#include "pionitem.h"

#include <QtGui>

 pionitem::pionitem()

 {
     setCursor(Qt::OpenHandCursor);
     setAcceptDrops (true);

 }

 QRectF pionitem::boundingRect() const
 {
     return QRectF(0, 0, 28, 28);
 }

 void pionitem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
 {
     Q_UNUSED(option);
     Q_UNUSED(widget);
     QPixmap pixmap3(":/images/pion_blanc.png");
     painter->drawPixmap(0,0,pixmap3);


 }
