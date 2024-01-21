
#ifndef WAVE_H
#define WAVE_H


#include <QObject>
#include <QGraphicsObject>
#include "player.h"

class Wave : public QGraphicsObject
{
    Q_OBJECT
    friend class GameScene;
public:
    Wave(Player &mainPlayer, bool isLuke, qreal attackFrequency, QGraphicsScene &scene, qreal attack);
    QPixmap lightsiberPixmap;
    QPixmap leftWavePixmap;
    QPixmap rightWavePixmap;

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void advance(int step);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void waveAttack();
    void flip(bool if_right);

private:
    Player &mainPlayer;
    QGraphicsScene &scene;
    QTimer attackTimer;
    QTimer waveTimer;
    qreal attackFrequency;
    qreal attack;
    bool isLuke;
    bool isRight;
    bool isDown;
    bool isBounce;
    int rangeUpLevel;

signals:
    void generateDrop(QPointF position);
};

#endif // WAVE_H
