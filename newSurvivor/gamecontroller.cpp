
#include "gamecontroller.h"
#include "mapitem.h"
#include "Global.h"
#include <QList>
#include <QGraphicsObject>
#include <QGraphicsView>
#include <QEvent>
#include <QKeyEvent>

void GameController::GenerateObstacle(qreal x, qreal y, obstacleSet type) {
    Obstacle *newObstacle = new Obstacle(mainPlayer, type);
    newObstacle -> setPos(x, y);
    QList<QGraphicsItem *> collisionsItems = scene.collidingItems(newObstacle);
    for(QGraphicsItem *item : collisionsItems) {
        if(item -> data(Type) == enemyType) {
            delete newObstacle;
            return;
        }
    }
    newObstacle -> setZValue(1);
    scene.addItem(newObstacle);
    ObstacleList.append(newObstacle);
    ObstacleInViewList.append(newObstacle);
    obstacleInView++;
}

GameController::GameController(QGraphicsScene &scene, const QList<MapItem *> &MapList, Player &mainPlayer, int gameLevel, QObject *parent):
    QObject(parent), scene(scene), MapList(MapList), mainPlayer(mainPlayer), W_(false), A_(false), S_(false), D_(false),
    W_block(false), A_block(false), S_block(false), D_block(false), obstacleInView(0), gameLevel(gameLevel)
{
    mainTimer.start(1000 / STEPS);
    obstacleTimer.start(10 * 1000);
    minion1Timer.start(2 * 1000);
    if(!mainPlayer.isLuke) {
        mainPlayer.shootTimer.start(mainPlayer.shootFrequency * 1000);
        connect(&mainPlayer.shootTimer, &QTimer::timeout, this, [&](){
            GeneratePlayerBullet(mainPlayer.if_right ? QPointF(WINDOW_WIDTH / 2 + 30, WINDOW_HEIGHT / 2) : QPointF(WINDOW_WIDTH / 2 - 30, WINDOW_HEIGHT / 2));
        });
    }

    initPlayer();

    playerCheckDamageTimer.start(1000 / 10);
    connect(&playerCheckDamageTimer, &QTimer::timeout, &mainPlayer, &Player::checkDamage);
//    winTimer.start(REMAIN_TIME * 1000);
//    remainTimer.start(1000);

    mainPool = new ItemPool(mainPlayer, gameLevel);

    connect(&mainTimer, &QTimer::timeout, &scene, &QGraphicsScene::advance);
    scene.installEventFilter(this);

    // 用于新生成障碍物
    QList<QGraphicsView *> views = scene.views();
    viewportRect = views.at(0) -> viewport() -> rect();

    // 每隔一秒检查一次屏幕中的障碍物是否少于三个，如果是，在屏幕外生成一个
    connect(&obstacleTimer, &QTimer::timeout, this, [=](){
        ObstacleInViewList.clear();
        for(int i = 0; i < ObstacleList.size(); i++) {
            QRectF itemRect = ObstacleList[i] -> sceneBoundingRect();
            if(viewportRect.intersects(itemRect)) {
                ObstacleInViewList.append(ObstacleList[i]);
            }
        }
        if(ObstacleInViewList.size() < 3) {
            if(W_) {
                GenerateObstacle(rand() % WINDOW_WIDTH, -150, static_cast<obstacleSet>(rand() % 3));
            }
            else if(A_) {
                GenerateObstacle(-150, rand() % WINDOW_HEIGHT, static_cast<obstacleSet>(rand() % 3));
            }
            else if(S_) {
                GenerateObstacle(rand() % WINDOW_WIDTH, WINDOW_HEIGHT, static_cast<obstacleSet>(rand() % 3));
            }
            else if(D_) {
                GenerateObstacle(WINDOW_WIDTH, rand() % WINDOW_HEIGHT, static_cast<obstacleSet>(rand() % 3));
            }
        }
    });

    // deal with the timers which control generating enemies
    connect(&minion1Timer, &QTimer::timeout, this, [=](){
        createEnemy(1);
    });



    // generate three obstacles firstly
    qreal x, y;
    for(int i = 0; i < 3; i++) {
        x = rand() % WINDOW_WIDTH;
        while (x > WINDOW_WIDTH / 2 - 250 && x < WINDOW_WIDTH / 2 + 100) {
            x = rand() % WINDOW_WIDTH;
        }
        y = rand() % (WINDOW_HEIGHT / 3) + i * (WINDOW_HEIGHT / 3);
        // qDebug() << x << " " << y;
        GenerateObstacle(x, y, static_cast<obstacleSet>(rand() % 3));
    }
}

GameController::~GameController() {
    delete remainTimeLabel;
    for(MapItem *item : MapList) {
        delete item;
    }
    for(Obstacle *item : ObstacleList) {
        delete item;
    }
    delete settings;
    delete mainPool;
}

bool GameController::eventFilter(QObject *object, QEvent *event)
{
    if (event -> type() == QEvent::KeyPress) {
        handleKeyPressed((QKeyEvent *)event);
        return true;
    }
    else if(event -> type() == QEvent::KeyRelease) {
        handleKeyReleased((QKeyEvent *)event);
        return true;
    }
    else {
        return QObject::eventFilter(object, event);
    }
}

void GameController::handleKeyPressed(QKeyEvent *event)
{
    // qDebug() << mobileItem::W;
    if (event->key() == Qt::Key_W)
    {
        W_ = true;
        mobileItem::W = true;
        mainPlayer.W = true;
        //qDebug() << mobileItem::W;
    }
    if (event->key() == Qt::Key_A)
    {
        A_ = true;
        mobileItem::A = true;
        mainPlayer.A = true;
        mainPlayer.flipPixmap(false);
    }
    if (event->key() == Qt::Key_S)
    {
        S_ = true;
        mobileItem::S = true;
        mainPlayer.S = true;
    }
    if (event->key() == Qt::Key_D)
    {
        D_ = true;
        mobileItem::D = true;
        mainPlayer.D = true;
        mainPlayer.flipPixmap(true);
    }
}

void GameController::handleKeyReleased(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W)
    {
        W_ = false;
        //S_block = false;
        mobileItem::W = false;
        mainPlayer.W = false;
    }
    if (event->key() == Qt::Key_A)
    {
        A_ = false;
        //D_block = false;
        mobileItem::A  =false;
        mainPlayer.A = false;
    }
    if (event->key() == Qt::Key_S)
    {
        S_ = false;
       // W_block = false;
        mobileItem::S = false;
        mainPlayer.S = false;
    }
    if (event->key() == Qt::Key_D)
    {
        D_ = false;
        //A_block = false;
        mobileItem::D = false;
        mainPlayer.D = false;
    }
}

void GameController::initPlayer() {
    settings = new QSettings(ARCHIVE_PATH, QSettings::IniFormat);
    bool if_ingame = settings -> value(QString("game/if_in_game"), false).toBool();
    if(if_ingame) {
        int remaintime = settings -> value(QString("game/remain_time"), 300).toInt();

        minion2countdownTimer.start((10 - (300 - remaintime)) * 1000 > 0 ? (10 - (300 - remaintime)) * 1000 : 1);
        minion3countdownTimer.start((150 - (300 - remaintime)) * 1000 > 0 ? (150 - (300 - remaintime)) * 1000 : 1);
        bosscountdownTimer.start((210 - (300 - remaintime)) * 1000 > 0 ? (210 - (300 - remaintime)) * 1000 > 0 : 1);
    }
    else {
        minion2countdownTimer.start(10 * 1000);
        minion3countdownTimer.start(90 * 1000);
        bosscountdownTimer.start(140 * 1000);
    }
    connect(&minion2countdownTimer, &QTimer::timeout, this, [=](){
        minion2Timer.start(3 * 1000);
        minion2countdownTimer.stop();
    });

    connect(&minion2Timer, &QTimer::timeout, this, [=](){
        createEnemy(2);
    });

    connect(&minion3countdownTimer, &QTimer::timeout, this, [=](){
        minion3Timer.start(4 * 1000);
        minion3countdownTimer.stop();
    });

    connect(&minion3Timer, &QTimer::timeout, this, [=](){
        createEnemy(3);
    });

    connect(&bosscountdownTimer, &QTimer::timeout, this, [=](){
        createEnemy(4);
    });
} // init the player's hp, speed and so on when the game starts

void GameController::createEnemy(int kind) {
    EnemyItem *newEnemy;
    switch(kind) {
    case 1:
        newEnemy = mainPool -> CreateEnemy1();
        break;
    case 2:
        newEnemy = mainPool -> CreateEnemy2();
        break;
    case 3:
        newEnemy = mainPool -> CreateEnemy3();
        break;
    case 4:
        newEnemy = mainPool -> CreateBoss();
        break;
    }
    scene.addItem(newEnemy);
    enemyList.append(newEnemy);
    connect(newEnemy, &EnemyItem::outRecycle, this, [=](){
        enemyList.removeOne(newEnemy);
        scene.removeItem(newEnemy);
        delete newEnemy;
    });
    connect(newEnemy, &EnemyItem::enemyShoot, this, &GameController::GenerateEnemyBullet);
    newEnemy->inView = true;
    int x = 0, y = 0;
    while(1) {
        switch(rand() % 4) {
        case 0:{
            x = - (newEnemy -> boundingRect().width());
            y = rand() % WINDOW_HEIGHT;
            break;
        }
        case 1:{
            x = rand() % WINDOW_WIDTH;
            y = -(newEnemy -> boundingRect().height());
            break;
        }
        case 2:{
            x = WINDOW_WIDTH;
            y = rand() % WINDOW_HEIGHT;
            break;
        }
        case 3:{
            x = rand() % WINDOW_WIDTH;
            y = WINDOW_HEIGHT;
            break;
        }
        }
        newEnemy -> setPos(x, y);
        bool collision = false;
        QList<QGraphicsItem *> collisionItems = scene.collidingItems(newEnemy);
        for(QGraphicsItem *item : collisionItems) {
            if(item -> data(Type) == obstacleType){
                collision = true;
                break;
            }
        }
        //qDebug() << collision;
        if(!collision) {
            break;
        }
    }
    int dx = WINDOW_WIDTH / 2 - x;
    int dy = WINDOW_HEIGHT / 2 - y;
    qreal length = qSqrt(dx * dx + dy * dy);
    // qDebug() << newEnemy -> pos();
    newEnemy -> setZValue(1);
    newEnemy -> setMoveDirection(dx / length, dy / length);
}

void GameController::GenerateEnemyBullet(QPointF position) {
    Bullet *newBullet = mainPool -> CreateEnemyBullet();
    newBullet -> setDetails(20, scene, position);
    connect(newBullet, &Bullet::bulletRecycle, this, [=](){
        scene.removeItem(newBullet);
        delete newBullet;
    });
    int dx = WINDOW_WIDTH / 2 - position.x();
    int dy = WINDOW_HEIGHT / 2 - position.y();
    qreal length = qSqrt(dx * dx + dy * dy);
    qreal rotation = qRadiansToDegrees(qAtan2(dy, dx));
    newBullet -> direction = QPointF(dx / length, dy / length);
    newBullet -> setRotation(rotation);
}

void GameController::GeneratePlayerBullet(QPointF position) {
    if(enemyList.isEmpty()) {
        return;
    }
    int num = 0;
    Bullet *newBullet;
    for(int i = 0; i < mainPlayer.shootNumber; i++) {
        newBullet = mainPool -> CreatePlayerBullet();

        newBullet -> setDetails(mainPlayer.shootAttack, scene, position);
        connect(newBullet, &Bullet::bulletRecycle, this, [=](){
            scene.removeItem(newBullet);
            delete newBullet;
        });
        int dx = enemyList[num]->pos().x() - WINDOW_WIDTH / 2;
        int dy = enemyList[num]->pos().y() - WINDOW_HEIGHT / 2;
        qreal length = qSqrt(dx * dx + dy * dy);
        qreal rotation = qRadiansToDegrees(qAtan2(dy, dx));
        newBullet -> direction = QPointF(dx / length, dy / length);
        newBullet -> setRotation(rotation);
        connect(newBullet, &Bullet::generateDrop, this, &GameController::createDrop);
        num = (num + 1) % enemyList.size();
    }

}

void GameController::GenerateDropItem(QPointF position, dropType type) {
    DropItem *newDrop;
    if(type == HP_DROP) {
        newDrop = mainPool -> CreateDropHp();
    }
    else if(type == MONEY_DROP)  {
        newDrop = mainPool -> CreateDropMoney();
    }
    else {
        newDrop = mainPool -> CreateDropExp();
    }
    scene.addItem(newDrop);
    newDrop -> setPos(position);
    connect(newDrop, &DropItem::DropRecycle, this, [=](){
        scene.removeItem(newDrop);
        delete newDrop;
    });
}

void GameController::createDrop(QPointF position) {
    switch(rand() % 6) {
    case 0: GenerateDropItem(position, MONEY_DROP); break;
    case 1: GenerateDropItem(position, HP_DROP); break;
    default: GenerateDropItem(position, EXP_DROP); break;
    }
}

void GameController::pause() {
    EnemyItem::isPause = true;
    playerCheckDamageTimer.stop();
}

void GameController::goon() {
    EnemyItem::isPause = false;
    playerCheckDamageTimer.start();
}


