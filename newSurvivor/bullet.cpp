
#include "bullet.h"
#include <QPainter>
#include <QGraphicsScene>
#include "Global.h"
#include "enemyitem.h"

Bullet::Bullet(bool isPlayer, Player &mainPlayer):
    mobileItem(mainPlayer), isAOE(false), isPlayer(isPlayer) {
    if(isPlayer) {
        bulletSpeed = 8;
        setData(ObjectData::Type, ObjectType::playerBulletType);
        bulletPixmap = QPixmap(":/images/blueBullet.png");
    }
    else {
        bulletSpeed = 5;
        setData(ObjectData::Type, ObjectType::enemyBulletType);
        bulletPixmap = QPixmap(":/images/redBullet.png");
    }

}

void Bullet::setDetails(qreal attack, QGraphicsScene &scene, QPointF position)
{
    this -> attack = attack;
    scene.addItem(this);
    setPos(position);
    setZValue(5);
}

QRectF Bullet::boundingRect() const{
    // 返回子弹图片的边界矩形
    return QRectF(-bulletPixmap.width() / 2, -bulletPixmap.height() / 2, bulletPixmap.width(), bulletPixmap.height());
}

QPainterPath Bullet::shape() const
{
    QPainterPath path;
    if(!isAOE)
        path.addRect(boundingRect());
    else
        path.addRect(boundingRect().adjusted(-40, -40, 40, 40));
    return path;
}

void Bullet::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(boundingRect().topLeft(), bulletPixmap);
}
void Bullet::advance(int step) {
    if(step == 0) {
        //handleCollisions();
        return;
    }
    else {
        move();
        moveAll();
        if(this -> x() > WINDOW_WIDTH || this -> x() < 0 || this -> y() < 0 || this -> y() > WINDOW_HEIGHT) {
            emit bulletRecycle();
            return;
        }
        if(isPlayer) {
            bool colliding = false;
            QList<QGraphicsItem *> collidingItems = scene() -> collidingItems(this);
            for(QGraphicsItem * item : collidingItems) {
                if(item -> data(ObjectData::Type) == ObjectType::enemyType) {
                    colliding = true;
                    EnemyItem *enemyItem = qgraphicsitem_cast<EnemyItem *>(item);
                    enemyItem->hp -= attack;
                    enemyItem -> startBlinking();
                    if(enemyItem->hp <= 0) {
                        emit generateDrop(enemyItem -> pos());
                        emit enemyItem -> outRecycle();
                    }
                    if(!isAOE){
                        emit bulletRecycle();
                        return;
                    }
                }
            }
            if(colliding) {
                emit bulletRecycle();
                return;
            }
        }
    }
}

void Bullet::move() {
    qreal x = pos().x();
    qreal y = pos().y();
    setPos(x + bulletSpeed * direction.x(), y + bulletSpeed * direction.y());
}
