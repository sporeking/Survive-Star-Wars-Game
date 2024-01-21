
#include "enemyitem.h"
#include "Global.h"
#include <QPainter>
#include <QGraphicsScene>

EnemyItem::EnemyItem(int level, int kind, Player &mainPlayer):
    mobileItem(mainPlayer), kind(kind), haveBullet(false), inView(false)
{
    if(level == 1) {
        // first level
        if(kind == 1) {
            attack = ENEMY_ATTACK_1;
            hp = ENEMY_HP_1;
            max_hp = ENEMY_HP_1;
            moveSpeed = ENEMY_SPEED_1;
            QPixmap *newPixmap = new QPixmap(":/images/minion.png");
            *newPixmap = newPixmap->scaled(QSize(newPixmap->width() * 0.7, newPixmap->height() * 0.7));
            pixmapList.append(newPixmap);
        }
        else if(kind == 2) {
            haveBullet = true;
            shootTimer.start(10 * 1000);
            attack = ENEMY_ATTACK_2;
            hp = ENEMY_HP_2;
            max_hp = ENEMY_HP_2;
            moveSpeed = ENEMY_SPEED_2;
            QPixmap *newPixmap = new QPixmap(":/images/jawa.png");
            *newPixmap = newPixmap->scaled(QSize(newPixmap->width() * 0.7, newPixmap->height() * 0.7));
            pixmapList.append(newPixmap);
        }
        else if(kind == 3) {
            attack = ENEMY_ATTACK_3;
            hp = ENEMY_HP_3;
            max_hp = ENEMY_HP_3;
            moveSpeed = ENEMY_SPEED_3;
            QPixmap *newPixmap = new QPixmap(":/images/minion3.png");
            *newPixmap = newPixmap->scaled(QSize(newPixmap->width() * 0.7, newPixmap->height() * 0.7));
            pixmapList.append(newPixmap);
        }
        else {
            attack = ENEMY_ATTACK_BOSS;
            hp = ENEMY_HP_BOSS;
            max_hp = ENEMY_HP_BOSS;
            moveSpeed = ENEMY_SPEED_BOSS1;
            QPixmap *newPixmap = new QPixmap(":/images/Jabar.png");
            *newPixmap = newPixmap->scaled(QSize(newPixmap->width() * 0.7, newPixmap->height() * 0.7));
            pixmapList.append(newPixmap);
        }
    }
    else if(level == 2) {
        if(kind == 1) {
            attack = ENEMY_ATTACK_1;
            hp = ENEMY_HP_1;
            max_hp = ENEMY_HP_1;
            moveSpeed = ENEMY_SPEED_1;
            QPixmap *newPixmap = new QPixmap(":/images/scout.png");
            *newPixmap = newPixmap->scaled(QSize(newPixmap->width() * 0.7, newPixmap->height() * 0.7));
            pixmapList.append(newPixmap);
        }
        else if(kind == 2) {
            haveBullet = true;
            shootTimer.start(10 * 1000);
            attack = ENEMY_ATTACK_2;
            hp = ENEMY_HP_2;
            max_hp = ENEMY_HP_2;
            moveSpeed = ENEMY_SPEED_2;
            QPixmap *newPixmap = new QPixmap(":/images/minion2.png");
            *newPixmap = newPixmap->scaled(QSize(newPixmap->width() * 0.7, newPixmap->height() * 0.7));
            pixmapList.append(newPixmap);
        }
        else if(kind == 3) {
            attack = ENEMY_ATTACK_3;
            hp = ENEMY_HP_3;
            max_hp = ENEMY_HP_3;
            moveSpeed = ENEMY_SPEED_3;
            QPixmap *newPixmap = new QPixmap(":/images/minion3.png");
            *newPixmap = newPixmap->scaled(QSize(newPixmap->width() * 0.7, newPixmap->height() * 0.7));
            pixmapList.append(newPixmap);
        }
        else {
            attack = ENEMY_ATTACK_BOSS;
            hp = ENEMY_HP_BOSS;
            max_hp = ENEMY_HP_BOSS;
            moveSpeed = ENEMY_SPEED_BOSS;
            QPixmap *newPixmap = new QPixmap(":/images/Vader.png");
            *newPixmap = newPixmap->scaled(QSize(newPixmap->width() * 0.7, newPixmap->height() * 0.7));
            pixmapList.append(newPixmap);
        }
    }
    else {
        if(kind == 1) {
            attack = ENEMY_ATTACK_1;
            hp = ENEMY_HP_1;
            max_hp = ENEMY_HP_1;
            moveSpeed = ENEMY_SPEED_1;
            QPixmap *newPixmap = new QPixmap(":/images/scout.png");
            *newPixmap = newPixmap->scaled(QSize(newPixmap->width() * 0.7, newPixmap->height() * 0.7));
            pixmapList.append(newPixmap);
        }
        else if(kind == 2) {
            haveBullet = true;
            shootTimer.start(8 * 1000);
            attack = ENEMY_ATTACK_2;
            hp = ENEMY_HP_2;
            max_hp = ENEMY_HP_2;
            moveSpeed = ENEMY_SPEED_2;
            QPixmap *newPixmap = new QPixmap(":/images/stormtrooper.png");
            *newPixmap = newPixmap->scaled(QSize(newPixmap->width() * 0.7, newPixmap->height() * 0.7));
            pixmapList.append(newPixmap);
        }
        else if(kind == 3) {
            haveBullet = true;
            shootTimer.start(4 * 1000);
            attack = ENEMY_ATTACK_3;
            hp = ENEMY_HP_3;
            max_hp = ENEMY_HP_3;
            moveSpeed = ENEMY_SPEED_3;
            QPixmap *newPixmap = new QPixmap(":/images/snowtrooper.png");
            *newPixmap = newPixmap->scaled(QSize(newPixmap->width() * 0.7, newPixmap->height() * 0.7));
            pixmapList.append(newPixmap);
        }
        else {
            attack = ENEMY_ATTACK_BOSS;
            hp = ENEMY_HP_BOSS;
            max_hp = ENEMY_HP_BOSS;
            moveSpeed = ENEMY_SPEED_BOSS;
            QPixmap *newPixmap = new QPixmap(":/images/Palpatine.png");
            *newPixmap = newPixmap->scaled(QSize(newPixmap->width() * 0.7, newPixmap->height() * 0.7));
            pixmapList.append(newPixmap);
        }
    }
    // walkTimer.start(1000 / speed);
    setData(ObjectData::Type, ObjectType::enemyType);
    connect(&shootTimer, &QTimer::timeout, this, [=](){
        if(!isPause)
            emit enemyShoot(pos());
    });
    isBlinking = false;
    blinkOpacity = 0.3;

    connect(&blinkTimer, &QTimer::timeout, this, &EnemyItem::stopBlinking);
}

bool EnemyItem::isPause = false;

QRectF EnemyItem::boundingRect() const {
    return pixmapList.at(0) -> rect();
}

QPainterPath EnemyItem::shape() const {
    QPainterPath path;
    path.addRect(boundingRect().adjusted(5, 10, -10, -5));
    return path;
}
void EnemyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (isBlinking) {
        painter->setOpacity(blinkOpacity);
        // 绘制闪烁效果
    }
    painter -> drawPixmap(0, 0, *pixmapList.at(pixmapNum));
    //qDebug() << "paint enemy" << this -> pos();
}

void EnemyItem::handleCollisions() {

}

void EnemyItem::move() {
    if(!inView) {
        return;
    }
    qreal x = pos().x();
    qreal y = pos().y();
    if(kind == 2) {
        if(x < -boundingRect().width() || x > WINDOW_WIDTH + boundingRect().width()) {
            qreal newX = -moveDirection.x();
            setMoveDirection(newX, moveDirection.y());
        }
        if(y < -boundingRect().height() || y > WINDOW_HEIGHT + boundingRect().height()) {
            qreal newY = -moveDirection.y();
            setMoveDirection(moveDirection.x(), newY);
        }
        setPos(x + moveSpeed * moveDirection.x(), y + moveSpeed * moveDirection.y());

    }
    else {
        setPos(x + moveSpeed * moveDirection.x(), y);
        bool l_block = false, v_block = false;
        QList<QGraphicsItem *> collisionItems = scene() -> collidingItems(this);
        for(QGraphicsItem * item : collisionItems) {
            if(item -> data(ObjectData::Type) == ObjectType::obstacleType) {
                l_block = true;
            }
        }
        setPos(x, y + moveSpeed * moveDirection.y());
        collisionItems = scene() -> collidingItems(this);
        for(QGraphicsItem * item : collisionItems) {
            if(item -> data(ObjectData::Type) == ObjectType::obstacleType) {
                v_block = true;
            }
        }
        if(l_block) {
            if(v_block) {
                setPos(x, y);
            }
            else {
                setPos(x, y + moveSpeed * moveDirection.y());
            }
        }
        else if(v_block) {
            setPos(x + moveSpeed * moveDirection.x(), y);
        }
        else {
            setPos(x + moveSpeed * moveDirection.x(), y + moveSpeed * moveDirection.y());
        }

        // make the new direction
        int dx = WINDOW_WIDTH / 2 - mainPlayer.boundingRect().width() / 2 - pos().x();
        int dy = WINDOW_HEIGHT / 2 - mainPlayer.boundingRect().height() / 2 - pos().y();
        qreal length = qSqrt(dx * dx + dy * dy);
        if(length == 0) {
            setMoveDirection(0, 0);
        }
        else {
            setMoveDirection(dx / length, dy / length);
        }
    }
}

void EnemyItem::advance(int step) {
    if(!inView) {
        return;
    }
    if(step == 0) {
        //handleCollisions();
        return;
    }
    else {
        move();
        moveAll();
        if(this -> x() > WINDOW_WIDTH + 800 || this -> x() < -800 || this -> y() < -450 || this -> y() > WINDOW_HEIGHT + 450) {
            emit outRecycle();
            return;
        }
    }
}

void EnemyItem::setMoveDirection(qreal x, qreal y) {
    moveDirection = QPointF(x, y);
}

int EnemyItem::getKind() {
    return kind;
}

QPointF EnemyItem::getMoveDirection() {
    return moveDirection;
}

qreal EnemyItem::getAttack() {
    return attack;
}

void EnemyItem::startBlinking() {
    isBlinking = true;
    blinkTimer.start(200);  // 设置闪烁时间
}

void EnemyItem::stopBlinking() {
    isBlinking = false;
    blinkTimer.stop();
    blinkOpacity = 1.0;
    update();  // 触发重绘
}


EnemyItem::~EnemyItem() {

}
