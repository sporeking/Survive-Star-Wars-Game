
#ifndef BULLET_H
#define BULLET_H


#include <QObject>
#include "mobileitem.h"
#include "player.h"

class Bullet : public mobileItem
{
    Q_OBJECT
    friend class Player;
    friend class GameController;
    friend class Wave;
    qreal attack;
    qreal bulletSpeed;
    QPointF direction;
    QPixmap bulletPixmap;

public:
    //explicit Bullet(QObject *parent = nullptr);
    Bullet(bool isPlayer, Player &mainPlayer);
    bool isAOE;
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void advance(int step);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void move();
    bool isPlayer;
    bool isExplode;
    void setDetails(qreal attack, QGraphicsScene &scene, QPointF position);

signals:
    void bulletRecycle();
    void generateDrop(QPointF position);
};

#endif // BULLET_H
