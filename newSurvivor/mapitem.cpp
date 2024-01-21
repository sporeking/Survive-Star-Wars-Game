
#include "mapitem.h"
#include "Global.h"
#include <QPixmap>
#include <QPainter>
#include <QKeyEvent>

MapItem::MapItem(int kind, Player &mainPlayer):mobileItem(mainPlayer) {
    if(kind == 1) {
        QPixmap *newPixmap = new QPixmap(":/images/yellow.jpg");
        pixmapList.append(newPixmap);
    }
    else if(kind == 2) {
        QPixmap *newPixmap = new QPixmap(":/images/green.jpg");
        pixmapList.append(newPixmap);
    }
    else {
        QPixmap *newPixmap = new QPixmap(":/images/grey.jpg");
        pixmapList.append(newPixmap);
    }
    setData(ObjectData::Type, (ObjectType::mapType));
}

QRectF MapItem::boundingRect() const
{
    return pixmapList.at(0) -> rect();
}

QPainterPath MapItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void MapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter -> drawPixmap(0, 0, *pixmapList.at(0));
    //qDebug() << "draw map";
}

void MapItem::advance(int step) {
    if(step == 0) {
        return;
    }
    moveAll();
    moveMap();
}

void MapItem::moveMap() {
    if(W) {
        qreal newY = pos().y();
        if (newY > WINDOW_HEIGHT) {
            newY -= 2 * WINDOW_HEIGHT;
        }
        setPos(pos().x(), newY);
    }
    if (S) {
        qreal newY = pos().y();
        if (newY < -WINDOW_HEIGHT) {
            newY += 2 * WINDOW_HEIGHT;
        }
        setPos(pos().x(), newY);
    }
    if (A) {
        qreal newX = pos().x();
        if (newX > WINDOW_WIDTH) {
            newX -= 2 * WINDOW_WIDTH;
        }
        setPos(newX, pos().y());
    }
    if (D) {
        qreal newX = pos().x();
        if (newX < -WINDOW_WIDTH) {
            newX += 2 * WINDOW_WIDTH;
        }
        setPos(newX, pos().y());
    }
}

//void MapItem::moveAll() {
//    if(W) {
//        qreal newY;
//        if(A != D){
//            newY = scenePos().ry() + 0.7 * speed;
//        }
//        else {
//            newY = scenePos().ry() + speed;
//            qDebug() << newY;
//        }
//        if (newY > WINDOW_HEIGHT) {
//            newY -= 2 * WINDOW_HEIGHT;
//        }
//        setPos(pos().x(), newY);
//        qDebug() << pos().x() << " " << pos().y();
//    }
//    if (S) {
//        qreal newY;
//        if(A != D){
//            newY = scenePos().ry() - 0.7 * speed;
//        }
//        else {
//            newY = pos().y() - speed;
//        }
//        if (newY < -WINDOW_HEIGHT) {
//            newY += 2 * WINDOW_HEIGHT;
//        }
//        setPos(pos().x(), newY);
//    }
//    if (A) {
//        qreal newX;
//        if(W != S) {
//            newX = pos().x() + 0.7 * speed;
//        }
//        else {
//            newX = pos().x() + speed;
//        }
//        if (newX > WINDOW_WIDTH) {
//            newX -= 2 * WINDOW_WIDTH;
//        }
//        setPos(newX, pos().y());
//    }
//    if (D) {
//        qreal newX;
//        if(W != S) {
//            newX = pos().x() - 0.7 * speed;
//        }
//        else {
//            newX = pos().x() - speed;
//        }
//        if (newX < -WINDOW_WIDTH) {
//            newX += 2 * WINDOW_WIDTH;
//        }
//        setPos(newX, pos().y());
//    }
//}
