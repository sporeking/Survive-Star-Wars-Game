
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Global.h"
#include "shopscene.h"
#include <QPainter>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setWindowIcon(QIcon(":/images/Luke.png"));
    setWindowTitle("Survive Star Wars");

    connect(ui -> actionQuit, &QAction::triggered, [=] () {
        this -> close();
    });

    QPushButton *shopButton = new QPushButton;
    shopButton -> setFixedSize(100, 50);
    shopButton -> setText("Shop");
    shopButton -> move(this -> width() * 0.5 - shopButton -> width() * 2, this -> height() * 0.5);
    shopButton -> setParent(this);
    QPushButton *playButton = new QPushButton;
    playButton -> setFixedSize(100, 50);
    playButton -> setText("Play");
    playButton -> move(this -> width() * 0.5 + playButton -> width(), this -> height() * 0.5);
    playButton -> setParent(this);

    shop = new ShopScene;

    connect(shopButton, &QPushButton::clicked, this, [=] () {
        shop -> show();
        this -> hide();
    });

    connect(shop, &ShopScene::backButtonClicked, this, [=] () {
        this -> show();
    });

    connect(playButton, &QPushButton::clicked, this, [=] () {
        levelChoose = new LevelChooseScene;

        connect(levelChoose, &LevelChooseScene::backButtonClicked, this, [=] () {
            this -> show();
        });

        connect(levelChoose, &LevelChooseScene::transitReturnMain, this, [=](){
            qDebug() << "main";
            this -> show();
        });
        this -> hide();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete shop;
    delete levelChoose;
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    QPixmap background_pix;
    background_pix.load(":/images/indexBackground.jpg");
    painter.drawPixmap(0, 0, this -> width(), this -> height(), background_pix);
}


