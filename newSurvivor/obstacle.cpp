
#include "obstacle.h"
#include "player.h"
#include "Global.h"
#include "enemyitem.h"
#include <QPainter>
#include <QGraphicsObject>
#include <QGraphicsScene>

Obstacle::Obstacle(Player &mainPlayer, obstacleSet type): mobileItem(mainPlayer)
{
    QPixmap *newPixmap;
    if(type == Falcon) {
        newPixmap = new QPixmap(":/images/Falcon.png");
    }
    else if(type == Ti) {
        newPixmap = new QPixmap(":/images/Ti.png");
    }
    else {
        newPixmap = new QPixmap(":/images/Xwings.png");
    }
    pixmapList.append(newPixmap);
    setData(ObjectData::Type, ObjectType::obstacleType);
    setZValue(1);
    // qDebug() << minX << " " << maxX;
}


QRectF Obstacle::boundingRect() const
{
    return pixmapList.at(0) -> rect();
}

QPainterPath Obstacle::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void Obstacle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter -> drawPixmap(0, 0, *pixmapList.at(0));
}

void Obstacle::advance(int step) {
    if(step == 0) {
        QList<QGraphicsItem *> collidingItems = this -> scene() -> collidingItems(this);
        for(QGraphicsItem * item : collidingItems) {
            if(item -> data(ObjectData::Type) == ObjectType::enemyType) {
                EnemyItem *enemyItem = qgraphicsitem_cast<EnemyItem *>(item);
                if(enemyItem -> getKind() == 2) {
                    if(!enemyItem -> collisionWithObstacle) {
                        QPointF dir = enemyItem ->getMoveDirection();
                        enemyItem -> setMoveDirection(-dir.x(), -dir.y());
                        enemyItem -> collisionWithObstacle = true;
                    }
                    else {
                        QPointF point = enemyItem -> pos();
                        enemyItem -> move();
                        if(!enemyItem -> collidesWithItem(this)) {
                            enemyItem -> collisionWithObstacle = false;
                        }
                        enemyItem -> setPos(point);
                    }
                }
                else {
                }
            }
        }
        return;
    }
    moveAll();
}

bool Obstacle::checkCollisions() {
//    if(mobileItem::block == true) {
//        return true;
//    }
    QPointF thePos = pos();
    moveAll();
    if(this->collidesWithItem(&mainPlayer)){
        setPos(thePos);
        return true;
    }
    else {
        setPos(thePos);
        return false;
    }
}
