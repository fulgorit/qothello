#ifndef SIMPLEITEM_H
#define SIMPLEITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QTextItem>
#include <QLabel>
#include <QGraphicsSceneMouseEvent>
#include <QLineEdit>
#include <QPushButton>
#include <QColor>


class SimpleItem : public QGraphicsItem
 {
 public:
      SimpleItem();

     QRectF boundingRect() const;
     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

 protected:
     void mousePressEvent(QGraphicsSceneMouseEvent *event);
     void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
     void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

 private:
     QColor color;
 };


/*
class SimpleItem : public QGraphicsItem
 {
 public:
     QRectF boundingRect() const
     {
         qreal penWidth = 1;
         return QRectF(-40 - penWidth / 2, -40 - penWidth / 2,
                       40 + penWidth, 40 + penWidth);
     }

     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
     {
         QPixmap pixmap("pion_vert.png");
         painter->drawPixmap(0,0,pixmap);

     }


};

*/


#endif // SIMPLEITEM_H
