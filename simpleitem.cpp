#include "simpleitem.h"
#include <QDrag>
#include <QMimeData>
#include <QtGui>


SimpleItem::SimpleItem()
     : color(qrand() % 256, qrand() % 256, qrand() % 256)
 {

     setFlag(QGraphicsItem::ItemIsMovable);
     setToolTip(QString("QColor(%1, %2, %3)\n%4")
                .arg(color.red()).arg(color.green()).arg(color.blue())
                .arg("Click and drag this color onto the robot!"));
     setCursor(Qt::OpenHandCursor);
 }

 QRectF SimpleItem::boundingRect() const
 {
     return QRectF(0.0, 0.0, 80, 80);
 }

void SimpleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   /*  Q_UNUSED(option);
     Q_UNUSED(widget);
     painter->setPen(Qt::NoPen);
     painter->setBrush(Qt::darkGray);
     painter->drawEllipse(-12, -12, 30, 30);
     painter->setPen(QPen(Qt::black, 1));
     painter->setBrush(QBrush(color));
     painter->drawEllipse(-15, -15, 30, 30);
*/
     QPixmap pixmap("pion_vert.png");
     painter->drawPixmap(0,0,pixmap);

}

 void SimpleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
 {
     if (event->button() != Qt::LeftButton) {
         event->ignore();
         return;
     }

     setCursor(Qt::ClosedHandCursor);
 }

 void SimpleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
 {
     if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton)).length() < QApplication::startDragDistance()) {
         return;
     }

     QDrag *drag = new QDrag(event->widget());
     QMimeData *mime = new QMimeData;
     drag->setMimeData(mime);

     QPixmap pixmap("pion_vert.png");
     drag->setPixmap(pixmap.scaled(80*0.47,80*0.47));
     drag->setHotSpot(QPoint(40*0.47, 40*0.47));


     drag->exec();
     setCursor(Qt::OpenHandCursor);
 }

 void SimpleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
 {
     setCursor(Qt::OpenHandCursor);
 }

