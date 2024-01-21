
#include "player.h"
#include "Global.h"
//#include "obstacle.h"
#include <QPainter>
#include <QDebug>
#include <QGraphicsScene>
#include "enemyitem.h"
#include "bullet.h"
#include "dropitem.h"

Player::Player(int kind):if_right(true), pixmap_num(0), level(1), shootNumber(1), W(false), A(false), S(false), D(false) {
    if(kind == 0) {
        max_hp = LUKE_HP;
        hp = LUKE_HP;
        speed = LUKE_SPEED;
        QPixmap LukePixmap(":/images/Luke.png");
        LukePixmap = LukePixmap.scaled(QSize(LukePixmap.width() * 0.7, LukePixmap.height() * 0.7));
        pixmap_list.append(LukePixmap);
        LukePixmap = QPixmap(":/images/Luke1.png");
        LukePixmap = LukePixmap.scaled(QSize(LukePixmap.width() * 0.7, LukePixmap.height() * 0.7));
        pixmap_list.append(LukePixmap);
        LukePixmap = QPixmap(":/images/Luke2.png");
        LukePixmap = LukePixmap.scaled(QSize(LukePixmap.width() * 0.7, LukePixmap.height() * 0.7));
        pixmap_list.append(LukePixmap);
    }
    else {
        isLuke = false;
        max_hp = HAN_HP;
        hp = HAN_HP;
        speed = HAN_SPEED;
        QPixmap HanPixmap(":/images/Han.png");
        HanPixmap = HanPixmap.scaled(QSize(HanPixmap.width() * 0.7, HanPixmap.height() * 0.7));
        pixmap_list.append(HanPixmap);
        HanPixmap = QPixmap(":/images/Han1.png");
        HanPixmap = HanPixmap.scaled(QSize(HanPixmap.width() * 0.7, HanPixmap.height() * 0.7));
        pixmap_list.append(HanPixmap);
        HanPixmap = QPixmap(":/images/Han2.png");
        HanPixmap = HanPixmap.scaled(QSize(HanPixmap.width() * 0.7, HanPixmap.height() * 0.7));
        pixmap_list.append(HanPixmap);
    }
    walkTimer.start(2000 / speed);
    connect(&walkTimer, &QTimer::timeout, this, [=](){
        if(A != D || W != S)
            pixmap_num = (pixmap_num++) % (pixmap_list.size() - 1) + 1;
        else {
            pixmap_num = 0;
        }
        update();
    });
    setData(ObjectData::Type, ObjectType::playerType);
    isBlinking = false;
    blinkOpacity = 0.3;

    connect(&blinkTimer, &QTimer::timeout, this, &Player::stopBlinking);
}


QRectF Player::boundingRect() const
{
    return pixmap_list.at(0).rect();
}

QPainterPath Player::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect().adjusted(5, 40, -16, -10));
    return path;
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (isBlinking) {
        painter->setOpacity(blinkOpacity);
    }
    painter->drawPixmap(0, 0, pixmap_list[pixmap_num]);
}

void Player::advance(int step) {
    if(step == 0) {
        checkCollisions();
        return;
    }
}

void Player::checkDamage() {
    QList<QGraphicsItem *> collisionItems = scene() -> collidingItems(this);
    for(QGraphicsItem *item : collisionItems) {
        if(item -> data(ObjectData::Type) == ObjectType::enemyType) {
            EnemyItem *enemyItem = qgraphicsitem_cast<EnemyItem *>(item);
            emit damaged(enemyItem -> getAttack());
            startBlinking();
        }
        else if(item -> data(ObjectData::Type) == ObjectType::enemyBulletType) {
            Bullet *bulletItem = qgraphicsitem_cast<Bullet *>(item);
            emit damaged(bulletItem -> attack);
            startBlinking();
            emit bulletItem -> bulletRecycle();
        }
        else if(item -> data(ObjectData::Type) == ObjectType::dropItemType) {
            DropItem *dropItem = qgraphicsitem_cast<DropItem *>(item);
            switch(dropItem -> type) {
            case HP_DROP:
                emit damaged(-30);
                emit dropItem -> DropRecycle();
                break;
            case EXP_DROP:
                emit getExp(20);
                emit dropItem -> DropRecycle();
                break;
            case MONEY_DROP:
                emit getMoney(50);
                emit dropItem -> DropRecycle();
                break;
            }
        }
    }
}

void Player::flipPixmap(bool right) {
    if(if_right != right) {
        for(int i = 0; i < pixmap_list.size(); i++){
            QImage newImage = pixmap_list.at(i).toImage();
            newImage = newImage.mirrored(true, false);
            pixmap_list[i] = QPixmap::fromImage(newImage);
        }
        if(if_right) {
            if_right = false;
        }
        else {
            if_right = true;
        }
    }
    emit flip(if_right);
}

void Player::checkCollisions() {
    qreal x, y;
    x = pos().x();
    y = pos().y();
    //
    setPos(x - speed, y);
    bool wb = false, ab = false, sb = false, db = false;
    QList<QGraphicsItem *> collisionItems = scene() -> collidingItems(this);
    for(QGraphicsItem * item : collisionItems) {
        if(item -> data(ObjectData::Type) == ObjectType::obstacleType) {
            mobileItem::A_block = true;
            ab = true;
            break;
        }
    }
    if(!ab) {
        mobileItem::A_block = false;
    }

    setPos(x + speed, y);
    collisionItems = scene() -> collidingItems(this);
    for(QGraphicsItem * item : collisionItems) {
        if(item -> data(ObjectData::Type) == ObjectType::obstacleType) {
            mobileItem::D_block = true;
            db = true;
            break;
        }
    }
    if(!db) {
        mobileItem::D_block = false;
    }

    setPos(x, y - speed);
    collisionItems = scene() -> collidingItems(this);
    for(QGraphicsItem * item : collisionItems) {
        if(item -> data(ObjectData::Type) == ObjectType::obstacleType) {
            mobileItem::W_block = true;
            wb = true;
            break;
        }
    }
    if(!wb) {
        mobileItem::W_block = false;
    }

    setPos(x, y + speed);
    collisionItems = scene() -> collidingItems(this);
    for(QGraphicsItem * item : collisionItems) {
        if(item -> data(ObjectData::Type) == ObjectType::obstacleType) {
            mobileItem::S_block = true;
            sb = true;
            break;
        }
    }
    if(!sb) {
        mobileItem::S_block = false;
    }

    // qDebug() << ab << wb << sb << db;

    setPos(x, y);
}

void Player::startBlinking() {
    isBlinking = true;
    blinkTimer.start(200);
}

void Player::stopBlinking() {
    isBlinking = false;
    blinkTimer.stop();
    blinkOpacity = 1.0;
    update();  // 触发重绘
}
