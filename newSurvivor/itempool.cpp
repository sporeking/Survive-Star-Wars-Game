
#include "itempool.h"
#include "Global.h"
#include <QGraphicsItem>
#include <QGraphicsScene>

//ItemPool::ItemPool(QObject *parent)
//    : QObject{parent}
//{

//}

//ItemPool* ItemPool::Instance() {
//    if(instance == nullptr) {
//        return instance = new ItemPool(mainPlayer);
//    }
//    else {
//        return instance;
//    }
//}

ItemPool::ItemPool(Player &mainPlayer, int gameLevel):
    mainPlayer(mainPlayer), gameLevel(gameLevel)
{
    for(int i = 0; i < 30; i++) {
        EnemyItem *enemy = new EnemyItem(gameLevel, 1, mainPlayer);
        enemyPool1.append(enemy);
    }
    for(int i = 0; i < 20; i++) {
        EnemyItem *enemy = new EnemyItem(gameLevel, 2, mainPlayer);
        enemyPool2.append(enemy);
    }
    for(int i = 0; i < 10; i++) {
        EnemyItem *enemy = new EnemyItem(gameLevel, 3, mainPlayer);
        enemyPool3.append(enemy);
    }
    for(int i = 0; i < 50; i++) {
        Bullet *bullet = new Bullet(false, mainPlayer);
        enemyBulletPool.append(bullet);
    }
    for(int i = 0; i < 60; i++) {
        DropItem *drop = new DropItem(mainPlayer, EXP_DROP);
        dropExpPool.append(drop);
    }
    for(int i = 0; i < 10; i++) {
        DropItem *drop = new DropItem(mainPlayer, HP_DROP);
        dropHpPool.append(drop);
    }
    for(int i = 0; i < 10; i++) {
        DropItem *drop = new DropItem(mainPlayer, MONEY_DROP);
        dropMoneyPool.append(drop);
    }
    if(!mainPlayer.isLuke) {
        for(int i = 0; i < 50; i++) {
            Bullet *bullet = new Bullet(true, mainPlayer);
            enemyBulletPool.append(bullet);
        }
    }
    boss = new EnemyItem(gameLevel, 4, mainPlayer);
}

ItemPool::~ItemPool() {
    for(EnemyItem *item : enemyPool1) {
        delete item;
    }
    for(EnemyItem *item : enemyPool2) {
        delete item;
    }
    for(EnemyItem *item : enemyPool3) {
        delete item;
    }
    for(Bullet *item : playerBulletPool) {
        delete item;
    }
    for(Bullet *item : enemyBulletPool) {
        delete item;
    }
    for(DropItem *item : dropExpPool) {
        delete item;
    }
    for(DropItem *item : dropHpPool) {
        delete item;
    }
    for(DropItem *item : dropMoneyPool) {
        delete item;
    }
    delete boss;
}

EnemyItem *ItemPool::CreateEnemy1() {
    if(enemyPool1.empty()) {
        return new EnemyItem(gameLevel, 1, mainPlayer);
    }
    EnemyItem *enemy = enemyPool1.first();
    enemyPool1.pop_front();

    return enemy;
}

EnemyItem *ItemPool::CreateEnemy2() {
    if(enemyPool2.empty()) {
        return new EnemyItem(gameLevel, 2, mainPlayer);
    }
    EnemyItem *enemy = enemyPool2.first();
    enemyPool2.pop_front();

    return enemy;
}

EnemyItem *ItemPool::CreateEnemy3() {
    if(enemyPool3.empty()) {
        return new EnemyItem(gameLevel, 3, mainPlayer);
    }
    EnemyItem *enemy = enemyPool3.first();
    enemyPool3.pop_front();

    return enemy;
}

EnemyItem *ItemPool::CreateBoss() {
    return boss;
}

Bullet *ItemPool::CreateEnemyBullet() {
    if(enemyBulletPool.empty()) {
        return new Bullet(false, mainPlayer);
    }
    Bullet *bullet = enemyBulletPool.first();
    enemyBulletPool.pop_front();

    return bullet;
}

Bullet *ItemPool::CreatePlayerBullet() {
    if(playerBulletPool.empty()) {
        return new Bullet(true, mainPlayer);
    }
    Bullet *bullet = playerBulletPool.first();
    playerBulletPool.pop_front();

    return bullet;
}

DropItem *ItemPool::CreateDropExp() {
    if(dropExpPool.empty()) {
        return new DropItem(mainPlayer, EXP_DROP);
    }
    DropItem *dropExp = dropExpPool.first();
    dropExpPool.pop_front();

    return dropExp;
}
DropItem *ItemPool::CreateDropHp() {
    if(dropHpPool.empty()) {
        return new DropItem(mainPlayer, HP_DROP);
    }
    DropItem *dropHp = dropHpPool.first();
    dropHpPool.pop_front();

    return dropHp;
}
DropItem *ItemPool::CreateDropMoney() {
    if(dropMoneyPool.empty()) {
        return new DropItem(mainPlayer, MONEY_DROP);
    }
    DropItem *dropMoney = dropMoneyPool.first();
    dropMoneyPool.pop_front();

    return dropMoney;
}

void ItemPool::recycleEnemyItem(EnemyItem *enemyItem) {
    enemyItem -> scene() -> removeItem(enemyItem);
    if(enemyItem->getKind() == 1) {
        enemyPool1.append(enemyItem);
    }
    else if(enemyItem->getKind() == 2) {
        enemyPool2.append(enemyItem);
    }
    else if(enemyItem->getKind() == 3) {
        enemyPool3.append(enemyItem);
    }
}

