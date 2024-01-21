
#ifndef ENEMYITEM_H
#define ENEMYITEM_H


#include <QObject>
#include "mobileitem.h"


class EnemyItem : public mobileItem
{
    Q_OBJECT
    friend class GameController;
    friend class Wave;
    friend class Bullet;
public:
   // explicit EnemyItem(QObject *parent = nullptr);
    EnemyItem(int level, int kind, Player &mainPlayer);
    ~EnemyItem();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void advance(int step);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void handleCollisions();
    void move();
    void checkDamage();
    void setMoveDirection(qreal x, qreal y);
    int getKind();
    QPointF getMoveDirection();
    bool collisionWithObstacle;
    qreal getAttack();

    void startBlinking();
    void stopBlinking();


protected:
    int kind;
    bool haveBullet;
    QTimer shootTimer;
    qreal hp;
    qreal max_hp;
    qreal attack;
    qreal moveSpeed; // real move speed, differ from the "speed" in "mobileItem"
    bool inView;
    static bool isPause;
    QPointF moveDirection;

    bool isBlinking;
    QTimer blinkTimer;
    qreal blinkOpacity;


signals:
    void outRecycle();
    void deadRecycle();
    void enemyShoot(QPointF position);
};

#endif // ENEMYITEM_H
