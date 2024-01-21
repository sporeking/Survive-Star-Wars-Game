
#ifndef GAMESCENE_H
#define GAMESCENE_H


#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMenuBar>
#include <QSettings>
#include "gamecontroller.h"
#include "player.h"
#include "wave.h"

class GameScene : public QMainWindow
{
    Q_OBJECT
    friend class GameController;
    int roleNum;
    int levelNum;
    int remainTime;
    int money;
    qreal money_coefficient;
    QTimer *winTimer;
    QTimer *remainTimer;
    QTimer *moneyTimer;
    QMenuBar *bar;
    QMenu *startMenu;
    QAction *quitAction;
    QPushButton *stopButton;
    QPushButton *backButton;
    QPushButton *goonButton;
    QPushButton *saveButton;
    QPushButton *returnMainButton;
    QLabel *remainTimeLabel;
    QLabel *levelLabel;
    QLabel *moneyLabel;
    QLabel *bloodBackgroundLabel;
    QLabel *bloodLabel;
    QLabel *expBackgroundLabel;
    QLabel *expLabel;

    QGraphicsScene *mainGameScene;
    QGraphicsView *mainGameView;
    GameController *mainController;
    QList<MapItem *> *MapList;

    QSettings *settings;

    Player *mainPlayer;
    Wave *mainWave;

    int hpUpLevel;
    int specialUpLevel;
    int attackUpLevel;
    int frequencyUpLevel;
    bool specialUp;
    QList<int> strongPool;
    QPushButton *chooseButton[3];

public:
    //explicit GameScene(QWidget *parent = nullptr);
    GameScene(int roleNum, int levelNum);
    ~GameScene();
    void LevelUp();
    void sceneStop();
    void sceneStart();

private:
    void setLabel(QLabel *label, qreal x, qreal y, QString labelString);
    void loseGame();

signals:
    void backButtonClicked();
    void sceneCreated();
    void returnMainWindow();
    void pause();

public slots:
    void changeBloodLabel(qreal damage);
    void changeMoneyLabel(int upMoney);
    void changeExpLabel(int upExp);

};

#endif // GAMESCENE_H
