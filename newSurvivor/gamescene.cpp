
#include "Global.h"
#include "gamescene.h"
#include "wave.h"
#include "mapitem.h"
#include <QMenuBar>
#include <QPushButton>
#include <QPainter>
#include <QTimer>
#include <QTimerEvent>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QSettings>
#include <QMessageBox>

GameScene::GameScene(int roleNum, int levelNum) :
    remainTime(REMAIN_TIME), money(0)
{
    emit sceneCreated();

    for(int i = 0; i < 13; i++){
        strongPool.append(i);
    }

    settings = new QSettings(ARCHIVE_PATH, QSettings::IniFormat);
    bool if_ingame = settings -> value(QString("game/if_in_game"), 0).toBool();
    if(if_ingame) {
        remainTime = settings -> value(QString("game/remain_time"), 300).toInt();
        money = settings -> value(QString("game/money"), 0).toInt();
        hpUpLevel = settings -> value(QString("game/hp_up_level"), 0).toInt();
        specialUpLevel = settings -> value(QString("game/speed_up_level"), 0).toInt();
        attackUpLevel = settings -> value(QString("game/attack_up_level"), 0).toInt();
        frequencyUpLevel = settings -> value(QString("game/frequency_up_level"), 0).toInt();
        specialUp = settings -> value(QString("game/special_up"), false).toBool();
        for(int i = hpUpLevel; i > 0; i--) {
            strongPool.removeOne(i - 1);
        }
        for(int i = attackUpLevel; i > 0; i--) {
            strongPool.removeOne(i + 2);
        }
        for(int i = specialUpLevel; i > 0; i--) {
            strongPool.removeOne(i + 5);
        }
        for(int i = frequencyUpLevel; i > 0; i--) {
            strongPool.removeOne(i + 8);
        }
        if(specialUp) {
            strongPool.removeOne(12);
        }
    }
    else {
        hpUpLevel = 0;
        specialUpLevel = 0;
        attackUpLevel = 0;
        frequencyUpLevel = 0;
        specialUp = false;
    }
    money_coefficient = (settings -> value(QString("shop/money_coefficient_level"), 1).toInt() - 1) * 0.5 + 1;
    this -> roleNum = roleNum;
    this -> levelNum = levelNum;

    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setWindowIcon(QIcon(":/images/Luke.png"));
    setWindowTitle("Survive Star Wars");

    mainGameView = new QGraphicsView;
    mainGameView -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mainGameView -> setRenderHint(QPainter::Antialiasing);
    mainGameView -> setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setCentralWidget(mainGameView);

    mainGameScene = new QGraphicsScene;

    qDebug() << mainGameView->viewport()->width() << mainGameView->viewport()->height();
    //mainGameScene -> setSceneRect(0, 0, mainGameView->viewport()->width(), mainGameView->viewport()->height());
    mainGameScene->setSceneRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    mainGameView -> setScene(mainGameScene);
    mainGameView -> resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    mainGameView -> setStyleSheet("padding:0px;border:0px");

    mainPlayer = new Player(roleNum);
    if(if_ingame) {
        mainPlayer -> isLuke = !settings -> value(QString("game/role"), true).toBool();
        mainPlayer -> isAOE = settings -> value(QString("game/special_up"), false).toBool();
        mainPlayer -> hp = settings->value(QString("game/hp")).toReal();
        mainPlayer -> max_hp = settings->value(QString("game/max_hp")).toReal();
        mainPlayer -> level = settings->value(QString("game/player_level"), 1).toInt();
        mainPlayer -> speed = settings->value(QString("game/speed")).toReal();
        mainPlayer -> relive_times = settings->value(QString("game/relive_times"), 0).toInt();
        mainPlayer -> exp = settings->value(QString("game/exp"), 0).toInt();
        mainPlayer -> shootFrequency = settings->value(QString("game/shoot_frequency"), 1).toReal();
    }
    else {
        mainPlayer->isLuke = !roleNum;
        int speed_level = settings -> value(QString("shop/speed_up_level"), 1).toInt();
        int hp_level = settings -> value(QString("shop/hp_up_level"), 1).toInt();
        int relive = settings -> value(QString("shop/relive_level"), 0).toInt();
        mainPlayer->speed += 0.8 * (speed_level - 1);
        mainPlayer->max_hp += 20 * (hp_level - 1);
        mainPlayer->hp += 20 * (hp_level - 1);
        mainPlayer->relive_times = relive;
        mainPlayer->exp = 0;
        mainPlayer->level = 1;
        mainPlayer->shootFrequency = 1;
    }
    mainPlayer -> shootAttack = 50 + 25 * attackUpLevel;

    MapItem *Map1 = new MapItem(levelNum, *mainPlayer);
    MapItem *Map2 = new MapItem(levelNum, *mainPlayer);
    MapItem *Map3 = new MapItem(levelNum, *mainPlayer);
    MapItem *Map4 = new MapItem(levelNum, *mainPlayer);
    mainGameScene -> addItem(Map1);
    mainGameScene -> addItem(Map2);
    mainGameScene -> addItem(Map3);
    mainGameScene -> addItem(Map4);
    Map1 -> setPos(0, 0);
    Map2 -> setPos(WINDOW_WIDTH, 0);
    Map3 -> setPos(0, WINDOW_HEIGHT);
    Map4 -> setPos(WINDOW_WIDTH, WINDOW_HEIGHT);
    Map1 -> setZValue(0);
    Map2 -> setZValue(0);
    Map3 -> setZValue(0);
    Map4 -> setZValue(0);

    MapList = new QList<MapItem *>;
    MapList -> append(Map1);
    MapList -> append(Map2);
    MapList -> append(Map3);
    MapList -> append(Map4);

    mainController = new GameController(*mainGameScene, *MapList, *mainPlayer, levelNum, this);

    mainGameScene -> addItem(mainPlayer);
    mainPlayer -> setPos(mainGameView->viewport()->width() / 2 - mainPlayer->boundingRect().width() / 2, mainGameView->viewport()->height() / 2 - mainPlayer->boundingRect().height() / 2);
    mainPlayer -> setZValue(3);

    if(roleNum == 0){
        mainWave = new Wave(*mainPlayer, true, 2 - frequencyUpLevel * 0.5, *mainGameScene, 50 + 25 * attackUpLevel);
        mainWave -> rangeUpLevel = specialUpLevel;
        if(specialUp) {
            mainWave->isBounce = true;
        }
        else {
            mainWave->isBounce = false;
        }
        connect(mainWave, &Wave::generateDrop, mainController, &GameController::createDrop);
    }
    else {
        mainPlayer -> shootTimer.stop();
        mainPlayer -> shootTimer.start((1 - 0.25 * frequencyUpLevel) * 1000);
        mainPlayer -> shootNumber = specialUpLevel + 1;
    }

    winTimer = new QTimer(this);
    winTimer -> start(remainTime * 1000);//剩余时间计时器
    remainTimer = new QTimer(this);
    remainTimer -> start(1000); // 显示剩余时间的计时器 1秒更新一次
    moneyTimer = new QTimer(this);
    moneyTimer -> start(5 * 1000);

    remainTimeLabel = new QLabel;
    levelLabel = new QLabel;
    moneyLabel = new QLabel;
    QString labelString = QString("%1:%2").arg(remainTime / 60).arg(remainTime % 60);
    setLabel(remainTimeLabel, 780, 40, labelString);
    labelString = QString("Level:%1").arg(mainPlayer->level);
    setLabel(levelLabel, 600, 40, labelString);
    labelString = QString("Money:%1").arg(money);
    setLabel(moneyLabel, 960, 40, labelString);

    bloodBackgroundLabel = new QLabel;
    bloodBackgroundLabel -> setParent(this);
    bloodBackgroundLabel -> setFixedSize(60, 10);
    bloodBackgroundLabel -> setStyleSheet("background-color: gray;");
    bloodBackgroundLabel -> move(this -> width() * 0.5 - bloodBackgroundLabel -> width() * 0.5, 370);

    bloodLabel = new QLabel;
    bloodLabel -> setParent(bloodBackgroundLabel);
    bloodLabel -> setFixedSize(60 * mainPlayer->hp / mainPlayer->max_hp, 10);
    bloodLabel -> setStyleSheet("background-color: red;");
    bloodLabel -> move(0, 0);

    connect(mainPlayer, &Player::damaged, this, &GameScene::changeBloodLabel);
    connect(mainPlayer, &Player::getExp, this, &GameScene::changeExpLabel);
    connect(mainPlayer, &Player::getMoney, this, &GameScene::changeMoneyLabel);

    expBackgroundLabel = new QLabel;
    expBackgroundLabel -> setParent(this);
    expBackgroundLabel -> setFixedSize(800, 15);
    expBackgroundLabel -> setStyleSheet("background-color: gray;");
    expBackgroundLabel -> move(this -> width() * 0.5 - expBackgroundLabel -> width() * 0.5, 100);

    expLabel = new QLabel;
    expLabel -> setParent(expBackgroundLabel);
    expLabel -> setFixedSize(800 * mainPlayer->exp / (100 + mainPlayer->level*10), 15);
    expLabel -> setStyleSheet("background-color: blue;");
    expLabel -> move(0, 0);

    stopButton = new QPushButton;
    stopButton -> setParent(this);
    stopButton -> setText("Stop");
    stopButton -> setFixedSize(100, 50);
    stopButton -> move(this -> width() * 0.9, this -> height() * 0.1);

    backButton = new QPushButton;
    backButton -> setParent(this);
    backButton -> setText("Back to choose Level");
    backButton -> setFixedSize(300, 100);
    backButton -> move(this -> width() * 0.5 - backButton -> width() * 0.5,
                     this -> height() * 0.5 - backButton -> height() * 0.5);
    backButton -> hide();

    goonButton = new QPushButton;
    goonButton -> setParent(this);
    goonButton -> setText("Go on!");
    goonButton -> setFixedSize(300, 100);
    goonButton -> move(this -> width() * 0.5 - goonButton -> width() * 0.5,
                     this -> height() * 0.5 - goonButton -> height() * 0.5);
    goonButton -> hide();

    saveButton = new QPushButton;
    saveButton -> setParent(this);
    saveButton -> setText("Save");
    saveButton -> setFixedSize(300, 100);
    saveButton -> move(this -> width() * 0.5 - saveButton -> width() * 0.5,
                     this -> height() * 0.5 + saveButton -> height() * 0.7);
    saveButton -> hide();

    returnMainButton = new QPushButton;
    returnMainButton -> setParent(this);
    returnMainButton -> setText("Return to Main Window");
    returnMainButton -> setFixedSize(300, 100);
    returnMainButton -> move(this -> width() * 0.5 - returnMainButton -> width() * 0.5,
                     this -> height() * 0.5 - returnMainButton -> height() * 2.2);
    returnMainButton -> hide();

    connect(remainTimer, &QTimer::timeout, this, [=] () {
        if(remainTime > 0)
            remainTime--;
        else{
            remainTime = 0;
            sceneStop();
            QMessageBox::StandardButton reply;
            reply = QMessageBox::information(nullptr, "Win!", "You win!", QMessageBox::Ok);
            if(reply == QMessageBox::Ok) {
                emit backButtonClicked();
                this -> deleteLater();
            }
            else {
            }
        }
        remainTimeLabel -> setText(QString("%1:%2").arg(remainTime / 60).arg(remainTime % 60));
    });

    connect(winTimer, &QTimer::timeout, this, [=] () {
        backButton -> show();
        winTimer -> stop();
        remainTimer -> stop();
        mainController -> mainTimer.stop();
        mobileItem::W = false;
        mobileItem::A = false;
        mobileItem::S = false;
        mobileItem::D = false;
        remainTimeLabel -> setText(QString("0:0"));
        QMessageBox::StandardButton reply;
        reply = QMessageBox::information(nullptr, "Win!", "You win!", QMessageBox::Ok);
        if(reply == QMessageBox::Ok) {
            emit backButtonClicked();
        }
        else {
        }
    });

    connect(moneyTimer, &QTimer::timeout, this, [=](){
        money += (int)(money_coefficient * 6);
        moneyLabel -> setText(QString("Money:%1").arg(money));
    });

    connect(stopButton, &QPushButton::clicked, this, [=](){
        sceneStop();
        stopButton -> hide();
        saveButton -> show();
        goonButton -> show();
        returnMainButton -> show();
    });

    connect(goonButton, &QPushButton::clicked, this, [=](){
        sceneStart();
        stopButton -> show();
        saveButton -> hide();
        goonButton -> hide();
        returnMainButton -> hide();
    });

    // save the game
    connect(saveButton, &QPushButton::clicked, this, [=] () {\
        bool inGame = true;
        settings -> setValue("game/if_in_game", inGame);
        settings -> setValue("game/remain_time", remainTime);
        settings -> setValue("game/game_level", levelNum);
        settings -> setValue("game/player_level", mainPlayer -> level);
        settings -> setValue("game/role", roleNum);
        settings -> setValue("game/max_hp", mainPlayer->max_hp);
        settings -> setValue("game/hp", mainPlayer->hp);
        settings -> setValue("game/speed", mainPlayer->speed);
        settings -> setValue("game/exp", mainPlayer->exp);
        settings -> setValue("game/relive_times", mainPlayer->relive_times);
        settings -> setValue("game/money", money);
        settings -> setValue("game/shoot_frequency", mainPlayer -> shootFrequency);
        int all_money = settings->value(QString("shop/money"), 0).toInt();
        settings -> setValue("shop/money", all_money + money);
        settings -> setValue("game/hp_up_level", hpUpLevel);
        settings -> setValue("game/speed_up_level", specialUpLevel);
        settings -> setValue("game/attack_up_level", attackUpLevel);
        settings -> setValue("game/frequency_up_level", frequencyUpLevel);
        settings -> setValue("game/special_up", specialUp);
        settings -> sync();
        QMessageBox::StandardButton reply = QMessageBox::information(nullptr, "Save", "You have saved the game.", QMessageBox::Ok);
        Q_UNUSED(reply);
    });

    connect(returnMainButton, &QPushButton::clicked, this, [=] () {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(nullptr, "Return", "Do you want to return to the main window?\nUnsaved game will be lost.", QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes) {
            emit returnMainWindow();
            this -> deleteLater();
        }
        else {
        }
    });

    connect(backButton, &QPushButton::clicked, this, [=] () {
        emit backButtonClicked();
    });
}

GameScene::~GameScene() {
    delete winTimer;
    delete remainTimer;
    delete moneyTimer;
    delete bar;
    delete startMenu;
    delete quitAction;
    delete stopButton;
    delete backButton;
    delete goonButton;
    delete saveButton;
    delete returnMainButton;
    delete remainTimeLabel;
    delete levelLabel;
    delete moneyLabel;
    delete bloodBackgroundLabel;
    delete bloodLabel;
    delete expBackgroundLabel;
    delete expLabel;
    delete mainGameScene;
    delete mainGameView;
    delete mainController;
    delete settings;
    delete mainPlayer;
    delete mainWave;
}

void GameScene::setLabel(QLabel *label, qreal x, qreal y, QString labelString) {
    label -> setParent(this);
    label -> setText(labelString);
    label -> setFixedSize(100, 50);
    QFont font("Times New Roman", 16);
    font.setWeight(QFont::Bold);
    label -> setFont(font);
    QPalette palette = label -> palette();
    palette.setColor(QPalette::WindowText, Qt::white);
    label -> setPalette(palette);
    label -> move(x, y);
    //label -> move(this -> width() * 0.5 - label -> width() * 0.5, this -> height() * 0.05);
}

void GameScene::changeBloodLabel(qreal damage) {
    if(mainPlayer -> hp <= 0) {
        return;
    }
    mainPlayer->hp -= damage;
    if(mainPlayer -> hp > mainPlayer -> max_hp) {
        mainPlayer -> hp = mainPlayer -> max_hp;
    }
    bloodLabel -> setFixedSize(60 * mainPlayer -> hp / mainPlayer -> max_hp, 10);
    if(mainPlayer->hp <= 0) {
        if(mainPlayer -> relive_times >= 1) {
            mainPlayer -> hp = mainPlayer -> max_hp;
            mainPlayer -> relive_times--;
            bloodLabel -> setFixedSize(60 * mainPlayer -> hp / mainPlayer -> max_hp, 10);
        }
        else {
            loseGame();
        }
    }
}

void GameScene::changeExpLabel(int upExp) {
    if(mainPlayer->level == 10) {
        levelLabel -> setText(QString("Level:MAX"));
        mainPlayer->exp = 0;
        expLabel -> setFixedSize(800, 10);
        return;
    }
    if(mainPlayer->exp + upExp >= 100 + mainPlayer->level * 25 && mainPlayer -> level != 10) {
        mainPlayer->exp = (mainPlayer->exp + upExp) % (100 + mainPlayer->level * 25);
        mainPlayer->level++;
        levelLabel -> setText(QString("Level:%1").arg(mainPlayer->level));
        qDebug() << mainPlayer->exp;
        qDebug() << mainPlayer->exp;
        expLabel -> setFixedSize(800 * mainPlayer->exp / (100 + mainPlayer->level * 25), 10);
        LevelUp();
    }
    else {
        mainPlayer->exp += upExp;
        expLabel -> setFixedSize(800 * mainPlayer->exp / (100 + mainPlayer->level * 25), 10);
    }
}

void GameScene::changeMoneyLabel(int upMoney) {
    money += upMoney;
    moneyLabel -> setText(QString("Money:%1").arg(money));
}

void GameScene::loseGame() {
    winTimer -> stop();
    remainTimer -> stop();
    mainController -> mainTimer.stop();
    bool ingame = false;
    settings->setValue("game/if_in_game", ingame);
    int all_money = settings->value(QString("shop/money"), 0).toInt();
    settings->setValue("shop/money", all_money + money);
    settings->sync();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::information(nullptr, "Lose!", "Failed to survive under the SITH...\nTry again!", QMessageBox::Ok);
    if(reply == QMessageBox::Ok) {
        emit backButtonClicked();
        this -> deleteLater();
    }
    else {
    }
}

void GameScene::sceneStop() {
    winTimer -> stop();
    remainTimer -> stop();
    stopButton -> hide();
    mainController -> mainTimer.stop();
    mobileItem::W = false;
    mobileItem::A = false;
    mobileItem::S = false;
    mobileItem::D = false;
    mainController -> pause();
}

void GameScene::sceneStart() {
    winTimer -> start();
    remainTimer -> start();
    stopButton -> show();
    mainController -> mainTimer.start();
    mainController -> goon();
}

void GameScene::LevelUp() {
    sceneStop();
    // qDebug() << strongPool;
    for(int i = 0; i < strongPool.size(); i++) {
        int between = strongPool[i];
        int r = rand() % strongPool.size();
        strongPool[i] = strongPool[r];
        strongPool[r] = between;
    }

    for(int i = 0; i < 3; i++) {
        chooseButton[i] = new QPushButton;
        chooseButton[i] -> setParent(this);
        switch(strongPool[i]) {
        case 0:
        case 1:
        case 2:
            chooseButton[i] -> setText("HP++");
            connect(chooseButton[i], &QPushButton::clicked, this, [=](){
                mainPlayer -> hp += 40;
                mainPlayer -> max_hp += 40;
                hpUpLevel++;
                changeBloodLabel(0);
                stopButton -> show();
                strongPool.removeAt(i);
                for(int i = 0; i < 3; i++) {
                    delete chooseButton[i];
                }
                sceneStart();
                mainGameScene->setFocus();
            });
            break;
        case 3:
        case 4:
        case 5:
            chooseButton[i] -> setText("ATTACK++");
            connect(chooseButton[i], &QPushButton::clicked, this, [=](){
                if(roleNum == 0) {
                    mainWave -> attack += 25;
                }
                else {
                    mainPlayer -> shootAttack += 25;
                }
                attackUpLevel++;
                stopButton -> show();
                strongPool.removeAt(i);
                for(int i = 0; i < 3; i++) {
                    delete chooseButton[i];
                }
                sceneStart();
                mainGameScene->setFocus();
            });
            break;
        case 6:
        case 7:
        case 8:
            if(roleNum == 0) {
                chooseButton[i] -> setText("LARGER RANGE");
            }
            else {
                chooseButton[i] -> setText("MORE BULLET");
            }
            connect(chooseButton[i], &QPushButton::clicked, this, [=](){
                if(roleNum == 0) {
                    mainWave->rangeUpLevel++;
                }
                else {
                    mainPlayer->shootNumber++;
                }
                specialUpLevel++;
                stopButton -> show();
                strongPool.removeAt(i);
                for(int i = 0; i < 3; i++) {
                    delete chooseButton[i];
                }
                sceneStart();
                mainGameScene->setFocus();
            });
            break;
        case 9:
        case 10:
        case 11:
            chooseButton[i] -> setText("ATTACK FREQUENCY++");
            connect(chooseButton[i], &QPushButton::clicked, this, [=](){
                frequencyUpLevel++;
                if(roleNum == 0) {
                    mainWave -> attackTimer.stop();
                    mainWave -> attackTimer.start((2 - frequencyUpLevel * 0.5) * 1000);
                }
                else {
                    mainPlayer -> shootTimer.stop();
                    mainPlayer -> shootTimer.start((1 - frequencyUpLevel * 0.25) * 1000);
                }
                stopButton -> show();
                strongPool.removeAt(i);
                for(int i = 0; i < 3; i++) {
                    delete chooseButton[i];
                }
                sceneStart();
                mainGameScene->setFocus();
            });
            break;
        case 12:
            if(roleNum == 0) {
                chooseButton[i] -> setText("BOUNCE BULLET");
            }
            else {
                chooseButton[i] -> setText("AOE BULLET");
            }
            connect(chooseButton[i], &QPushButton::clicked, this, [=](){
                specialUp = true;
                if(roleNum == 0) {
                    mainWave->isBounce = true;
                }
                else {
                    mainPlayer -> isAOE = true;
                }
                stopButton -> show();
                strongPool.removeAt(i);
                for(int i = 0; i < 3; i++) {
                    delete chooseButton[i];
                }
                sceneStart();
                mainGameScene->setFocus();
            });
            break;
        }
        qDebug() << "666";
        chooseButton[i] -> setFixedSize(300, 100);
        chooseButton[i] -> move(this -> width() * 0.5 - chooseButton[i] -> width() * 0.5 + (i - 1) * 500,
                         this -> height() * 0.5 - chooseButton[i] -> height() * 0.5);
        chooseButton[i] -> show();
    }
}
