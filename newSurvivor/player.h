
#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsObject>
#include <QTimer>

class Player : public QGraphicsObject
{
    Q_OBJECT
    friend class GameController;
    friend class GameScene;
public:
    // explicit Player(QObject *parent = nullptr);
    Player(int kind);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void advance(int step);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void checkDamage();
    void checkCollisions();
    qreal getSpeed() {
        return speed;
    }
    void flipPixmap(bool right);
    bool if_right;
    bool isLuke;

    void startBlinking();
    void stopBlinking();


private:
    qreal max_hp, hp;
    qreal speed;
    int pixmap_num;
    int relive_times;
    int level;
    int exp;
    QList<QPixmap> pixmap_list;
    QTimer walkTimer;
    // about han solo shoot
    QTimer shootTimer;
    qreal shootFrequency;
    bool isAOE;
    int shootAttack;
    int shootNumber;
    bool W, A, S, D;

    bool isBlinking;
    QTimer blinkTimer;
    qreal blinkOpacity;
signals:
    void damaged(qreal damage);
    void flip(bool isRight);
    void getExp(int upExp);
    void getMoney(int upMoney);
};

#endif // PLAYER_H
