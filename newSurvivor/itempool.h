
#ifndef ITEMPOOL_H
#define ITEMPOOL_H


#include <QObject>
#include "enemyitem.h"
#include "bullet.h"
#include "player.h"
#include "dropitem.h"


class ItemPool : public QObject
{
    Q_OBJECT
//    static ItemPool *instance;
public:
//    static ItemPool *Instance();

    ItemPool(Player &mainPlayer, int gameLevel);
    EnemyItem *CreateEnemy1();
    EnemyItem *CreateEnemy2();
    EnemyItem *CreateEnemy3();
    EnemyItem *CreateBoss();
    Bullet *CreateEnemyBullet();
    Bullet *CreatePlayerBullet();
    DropItem *CreateDropExp();
    DropItem *CreateDropHp();
    DropItem *CreateDropMoney();
    ~ItemPool();

    void recycleEnemyItem(EnemyItem *enemyItem);

protected:
    QList<EnemyItem *> enemyPool1;
    QList<EnemyItem *> enemyPool2;
    QList<EnemyItem *> enemyPool3;
    QList<Bullet *> playerBulletPool;
    QList<Bullet *> enemyBulletPool;
    QList<DropItem *> dropExpPool;
    QList<DropItem *> dropHpPool;
    QList<DropItem *> dropMoneyPool;
    EnemyItem *boss;
    Player &mainPlayer;
    int gameLevel;

signals:

};

#endif // ITEMPOOL_H
