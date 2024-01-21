
#ifndef MAPITEM_H
#define MAPITEM_H


#include "mobileitem.h"

class MapItem : public mobileItem
{
    Q_OBJECT
    friend class GameController;
    friend class GameScene;
public:
    //explicit MapItem(QObject *parent = nullptr);
    MapItem(int kind, Player &mainPlayer);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    //void moveAll();
    //void keyPressEvent(QKeyEvent *event);
    //void keyReleaseEvent(QKeyEvent *event);
    void moveMap();

private:
    void advance(int step);
    // bool W, A, S, D;
    qreal speed;

signals:

};

#endif // MAPITEM_H
