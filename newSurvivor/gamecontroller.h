
#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H


#include <QObject>
#include <QTimer>
#include "Global.h"
#include <QGraphicsScene>
#include "mapitem.h"
#include "player.h"
#include "obstacle.h"
#include "enemyitem.h"
#include "itempool.h"
#include <QKeyEvent>
#include <QLabel>
#include <QSettings>


class GameController : public QObject
{
    Q_OBJECT
    friend class GameScene;
public:
    //explicit GameController(QObject *parent = nullptr);
    GameController(QGraphicsScene &scene, const QList<MapItem *> &MapList, Player &mainPlayer, int gameLevel, QObject *parent = 0);
    ~GameController();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    bool eventFilter(QObject *object, QEvent *event);
    void handleKeyPressed(QKeyEvent *event);
    void handleKeyReleased(QKeyEvent *event);
    void GenerateObstacle(qreal x, qreal y, obstacleSet type);
    void initPlayer(); // init the player's hp, speed and so on when the game starts
    void GenerateEnemyBullet(QPointF position);
    void GeneratePlayerBullet(QPointF position);
    void GenerateDropItem(QPointF position, dropType type);
    void pause();
    void goon();

private:
    QTimer mainTimer;
    QTimer obstacleTimer;
    QTimer minion1Timer;
    QTimer minion2countdownTimer;
    QTimer minion2Timer;
    QTimer minion3countdownTimer;
    QTimer minion3Timer;
    QTimer bosscountdownTimer;
    QTimer playerCheckDamageTimer;
//    QTimer winTimer;
//    QTimer remainTimer;
//    int remainTime;
    QLabel *remainTimeLabel;
    QGraphicsScene &scene;
    const QList<MapItem *> MapList;
    QList<Obstacle *> ObstacleList;
    QList<Obstacle *> ObstacleInViewList;
    QList<EnemyItem *> enemyList;
//    QList<EnemyItem *> enemyList;
    Player &mainPlayer;
    bool W_, A_, S_, D_;
    bool W_block, A_block, S_block, D_block;
    //void moveMap(direction horizontalDirection, direction verticalDirection);
    QSettings *settings;
    int obstacleInView;
    QRectF viewportRect;

    ItemPool *mainPool;
    int gameLevel;


signals:

public slots:
//    void makeBlock(direction dir);
//    void cancelBlock(direction dir);
    void createEnemy(int kind);
    void createDrop(QPointF position);
};

#endif // GAMECONTROLLER_H
