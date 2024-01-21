
#include "levelchoosescene.h"
#include "Global.h"
#include "mybutton.h"
#include "gamescene.h"
#include <QMenuBar>
#include <QPainter>
#include <QPushButton>
#include <QMessageBox>

LevelChooseScene::LevelChooseScene(QWidget *parent)
    : QMainWindow{parent}
{
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setWindowIcon(QIcon(":/images/Luke.png"));
    setWindowTitle("Survive Star Wars");

    QMenuBar *bar = menuBar();
    setMenuBar(bar);
    QMenu *startMenu = bar -> addMenu("Start");
    QAction *quitAction = startMenu -> addAction("Quit");

    connect(quitAction, &QAction::triggered, [=] () {
        this -> close();
    });

    QPushButton *backButton = new QPushButton;
    backButton -> setParent(this);
    backButton -> setText("Back");
    backButton -> setFixedSize(100, 50);
    backButton -> move(this -> width() * 0.9, this -> height() * 0.9);

    connect(backButton, &QPushButton::clicked, [=] () {
        this->hide();
        backButtonClicked();
    });

    roleNum = 0;
    levelNum = 1;

    MyButton *chooseLukeButton = new MyButton(":/images/Luke.png");
    chooseLukeButton -> setParent(this);
    chooseLukeButton -> setFixedSize(chooseLukeButton -> iconwidth * 3, chooseLukeButton -> iconheight * 3);
    chooseLukeButton -> move(this -> width() * 0.25, this -> height() * 0.25);
    chooseLukeButton -> clickedDown();

    MyButton *chooseHanButton = new MyButton(":/images/Han.png");
    chooseHanButton -> setParent(this);
    chooseHanButton -> setFixedSize(chooseHanButton -> iconwidth * 3, chooseHanButton -> iconheight * 3);
    chooseHanButton -> move(this -> width() * 0.25, this -> height() * 0.65);
    chooseHanButton -> clickedUp();

    connect(chooseLukeButton, &MyButton::clicked, [=] () {
        roleNum = 0;
        chooseLukeButton -> clickedDown();
        chooseHanButton -> clickedUp();
    });

    connect(chooseHanButton, &MyButton::clicked, [=] () {
        roleNum = 1;
        chooseHanButton -> clickedDown();
        chooseLukeButton -> clickedUp();
    });

    MyButton *chooseFirstLevelButton = new MyButton(":/images/yellow.jpg", LEVEL_BUTTON_WIDTH, LEVEL_BUTTON_HEIGHT);
    chooseFirstLevelButton -> setParent(this);
    chooseFirstLevelButton -> setFixedSize(LEVEL_BUTTON_WIDTH, LEVEL_BUTTON_HEIGHT);
    chooseFirstLevelButton -> move(this -> width() * 0.5 - LEVEL_BUTTON_WIDTH * 0.5, this -> height() * 0.3);
    chooseFirstLevelButton -> clickedDown();

    MyButton *chooseSecondLevelButton = new MyButton(":/images/green.jpg", LEVEL_BUTTON_WIDTH, LEVEL_BUTTON_HEIGHT);
    chooseSecondLevelButton -> setParent(this);
    chooseSecondLevelButton -> setFixedSize(LEVEL_BUTTON_WIDTH, LEVEL_BUTTON_HEIGHT);
    chooseSecondLevelButton -> move(this -> width() * 0.5 - LEVEL_BUTTON_WIDTH * 0.5, this -> height() * 0.5);
    chooseSecondLevelButton -> clickedUp();

    MyButton *chooseThirdLevelButton = new MyButton(":/images/grey.jpg", LEVEL_BUTTON_WIDTH, LEVEL_BUTTON_HEIGHT);
    chooseThirdLevelButton -> setParent(this);
    chooseThirdLevelButton -> setFixedSize(LEVEL_BUTTON_WIDTH, LEVEL_BUTTON_HEIGHT);
    chooseThirdLevelButton -> move(this -> width() * 0.5 - LEVEL_BUTTON_WIDTH * 0.5, this -> height() * 0.7);
    chooseThirdLevelButton -> clickedUp();

    connect(chooseFirstLevelButton, &MyButton::clicked, [=] () {
        chooseFirstLevelButton -> clickedDown();
        chooseSecondLevelButton -> clickedUp();
        chooseThirdLevelButton -> clickedUp();
        levelNum = 1;
    });

    connect(chooseSecondLevelButton, &MyButton::clicked, [=] () {
        chooseFirstLevelButton -> clickedUp();
        chooseSecondLevelButton -> clickedDown();
        chooseThirdLevelButton -> clickedUp();
        levelNum = 2;
    });

    connect(chooseThirdLevelButton, &MyButton::clicked, [=] () {
        chooseFirstLevelButton -> clickedUp();
        chooseSecondLevelButton -> clickedUp();
        chooseThirdLevelButton -> clickedDown();
        levelNum = 3;
    });

    QPushButton *startGameButton = new QPushButton;
    startGameButton -> setParent(this);
    startGameButton -> setText("Start!");
    startGameButton -> setFixedSize(100, 50);
    startGameButton -> move(this -> width() * 0.8, this -> height() * 0.5);


    connect(startGameButton, &QPushButton::clicked, [=] () {
        game = new GameScene(roleNum, levelNum);
        this -> hide();
        game -> show();
        connect(game, &GameScene::backButtonClicked, [=] () {
            this->show();
            game->hide();
            game->deleteLater();
            game = nullptr;
        });
        connect(game, &GameScene::sceneCreated, this, [=](){
            this -> hide();
        });

        connect(game, &GameScene::returnMainWindow, this, [=](){
            qDebug() << "transit";
            emit transitReturnMain();
        });
    });

    this -> hide();

    QSettings settings(ARCHIVE_PATH, QSettings::IniFormat);
    bool if_continue = settings.value(QString("game/if_in_game"), false).toBool();
    if(if_continue) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(nullptr, "Continue?", "Do you want to continue last game?\nIf you choose no you will lose your last game.", QMessageBox::Yes | QMessageBox::No);

        if(reply == QMessageBox::Yes) {
            int role = settings.value(QString("game/role"), 1).toInt();
            int level = settings.value(QString("game/game_level"), 1).toInt();
            game = new GameScene(role, level);
            game -> show();
            this -> hide();
            connect(game, &GameScene::backButtonClicked, [=] () {
                this->show();
                game->hide();
                game->deleteLater();
                game = nullptr;
            });
        }
        else {
            bool myBool = false;
            settings.setValue("game/if_in_game", myBool);
            settings.sync();
            this -> show();
        }
    }
    else {
        this -> show();
    }
    if_continue = settings.value(QString("game/if_in_game"), false).toBool();
}

void LevelChooseScene::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    QPixmap background_pix;
    background_pix.load(":/images/indexBackground.jpg");
    painter.drawPixmap(0, 0, this -> width(), this -> height(), background_pix);
}
