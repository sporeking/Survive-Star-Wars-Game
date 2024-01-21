
#ifndef DROPITEM_H
#define DROPITEM_H


#include <QObject>
#include "mobileitem.h"
#include "Global.h"

class DropItem : public mobileItem
{
    Q_OBJECT
    friend class Player;
public:
    // explicit Obstacle(QObject *parent = nullptr);
    DropItem(Player &mainPlayer, dropType type);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void advance(int step);
    dropType type;

private:
    QPixmap dropPixmap;

signals:
    void DropRecycle();
};

#endif // DROPITEM_H
