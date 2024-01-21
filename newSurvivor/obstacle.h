
#ifndef OBSTACLE_H
#define OBSTACLE_H


#include "mobileitem.h"
#include "player.h"
#include "Global.h"

class Obstacle : public mobileItem
{
    Q_OBJECT
    friend class GameController;
public:
    // explicit Obstacle(QObject *parent = nullptr);
    Obstacle(Player &mainPlayer, obstacleSet type);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    void advance(int step);
    bool checkCollisions();

signals:
    void block(direction dir);
    void cancelBlock(direction dir);
};

#endif // OBSTACLE_H
