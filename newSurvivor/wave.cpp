
#include "wave.h"
#include "Global.h"
#include "enemyitem.h"
#include "bullet.h"
#include <QPainter>
#include <QGraphicsScene>

//Wave::Wave(QObject *parent)
//    : QObject{parent}
//{

//}

Wave::Wave(Player &mainPlayer, bool isLuke, qreal attackFrequency, QGraphicsScene &scene, qreal attack):
    mainPlayer(mainPlayer), scene(scene), attackFrequency(attackFrequency), attack(attack), isLuke(isLuke), isDown(false), rangeUpLevel(0) {
    if(isLuke) {
        lightsiberPixmap = QPixmap(":/images/blueLightsiber.png");
        lightsiberPixmap = lightsiberPixmap.scaled(QSize(lightsiberPixmap.width() * 0.7, lightsiberPixmap.height() * 0.7));
        rightWavePixmap = QPixmap(":/images/bluewave1.png");
        rightWavePixmap = rightWavePixmap.scaled(QSize(rightWavePixmap.width() * 0.7, rightWavePixmap.height() * 0.7));
        leftWavePixmap = QPixmap(":/images/bluewave2.png");
        leftWavePixmap = leftWavePixmap.scaled(QSize(leftWavePixmap.width() * 0.7, leftWavePixmap.height() * 0.7));
    }
    else {
        lightsiberPixmap = QPixmap(":/images/redLightsiber.png");
        lightsiberPixmap = lightsiberPixmap.scaled(QSize(lightsiberPixmap.width() * 0.7, lightsiberPixmap.height() * 0.7));
        rightWavePixmap = QPixmap(":/images/redwave1.png");
        rightWavePixmap = rightWavePixmap.scaled(QSize(rightWavePixmap.width() * 0.7, rightWavePixmap.height() * 0.7));
        leftWavePixmap = QPixmap(":/images/redwave2.png");
        leftWavePixmap = leftWavePixmap.scaled(QSize(leftWavePixmap.width() * 0.7, leftWavePixmap.height() * 0.7));
    }

    scene.addItem(this);
    setPos(mainPlayer.x() + 46, mainPlayer.y());
    setZValue(5);

    attackTimer.start(1000 * attackFrequency);

    connect(&attackTimer, &QTimer::timeout, this, &Wave::waveAttack);

    connect(&mainPlayer, &Player::flip, this, &Wave::flip);
}

QRectF Wave::boundingRect() const {
    if(isRight) {
        return rightWavePixmap.rect();
    }
    else {
        return leftWavePixmap.rect();
    }
}

QPainterPath Wave::shape() const {
    QPainterPath path;
    path.addRect(boundingRect().adjusted(-30 * rangeUpLevel, -30 * rangeUpLevel, 30 * rangeUpLevel, 30 * rangeUpLevel));
    return path;
}
void Wave::advance(int step) {
    Q_UNUSED(step);
}
void Wave::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(isDown) {
        if(isRight) {
            painter->drawPixmap(0, 0, rightWavePixmap);
        }
        else {
            painter -> drawPixmap(0, 0, leftWavePixmap);
        }
    }
    else {
        if(isRight) {
            painter -> drawPixmap(0, 0, lightsiberPixmap);
        }
        else {
            painter -> drawPixmap(leftWavePixmap.width() - lightsiberPixmap.width(), 0, lightsiberPixmap);
        }
    }
}

void Wave::waveAttack() {
    isDown = true;
    waveTimer.start(100);
    connect(&waveTimer, &QTimer::timeout, this, [=](){
        isDown = false;
        waveTimer.stop();
    });
    QList<QGraphicsItem *> collidingItems = scene.collidingItems(this);
    for(QGraphicsItem * item : collidingItems) {
        if(item -> data(ObjectData::Type) == ObjectType::enemyType) {
            EnemyItem *enemyItem = qgraphicsitem_cast<EnemyItem *>(item);
            enemyItem->hp -= attack;
            enemyItem->startBlinking();
            if(enemyItem->hp <= 0) {
                emit generateDrop(enemyItem -> pos());
                emit enemyItem -> outRecycle();
            }
        }
        else if(isBounce && item -> data(ObjectData::Type) == ObjectType::enemyBulletType) {
            Bullet *bulletItem = qgraphicsitem_cast<Bullet *>(item);
            qreal x = - bulletItem -> direction.x();
            qreal y = - bulletItem -> direction.y();
            bulletItem -> direction = QPointF(x, y);
            bulletItem -> setData(ObjectData::Type, ObjectType::playerBulletType);
            bulletItem -> isPlayer = true;
        }
    }
}

void Wave::flip(bool if_right) {
    if(if_right) {
        isRight = true;
        setPos(mainPlayer.x() + 46, mainPlayer.y());
    }
    else {
        isRight = false;
        setPos(mainPlayer.x() - 95, mainPlayer.y());
    }
}
