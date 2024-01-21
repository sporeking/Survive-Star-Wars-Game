
#ifndef MOBILEITEM_H
#define MOBILEITEM_H


#include <QObject>
#include <QGraphicsItem>
#include <QRectF>
#include <QWidget>
#include <QTimer>
#include "player.h"

class mobileItem : public QGraphicsObject
{
    Q_OBJECT
    friend class GameScene;
    friend class GameController;
    friend class ItemPool;
    friend class Player;
public:
    //explicit mobileItem(QObject *parent = nullptr);
    mobileItem(Player &mainPlayer);
//    virtual QRectF boundingRect() const;
//    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void moveAll();

protected:
    virtual void advance(int step) = 0;
    QList<QPixmap *> pixmapList;
    int pixmapNum;
    Player &mainPlayer;
    qreal speed;
    static bool W, A, S, D;
    static bool W_block, A_block, S_block, D_block;

    // GameController &mainController;

signals:

};

#endif // MOBILEITEM_H
