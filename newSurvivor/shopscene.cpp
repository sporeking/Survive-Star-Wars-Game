
#include "shopscene.h"
#include "Global.h"
#include <QMenuBar>
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include <QSettings>
#include <QMessageBox>
#include <QDir>

void ShopScene::setLabel(QLabel *label, QString &type_level, qreal wid, qreal hei) {
    label -> setParent(this);
    label -> setText(type_level);
    label -> setFixedSize(200, 50);
    QFont ft;
    ft.setPointSize(12);
    label -> setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::white);
    label -> setPalette(pa);
    label -> move(this -> width() * wid, this -> height() * hei);
}

ShopScene::ShopScene(QWidget *parent)
    : QMainWindow{parent}
{
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setWindowIcon(QIcon(":/images/Luke.png"));
    setWindowTitle("Survive Star Wars");

    QMenuBar *bar = menuBar();
    setMenuBar(bar);
    QMenu *startMenu = bar -> addMenu("Start");
    QAction *quitAction = startMenu -> addAction("Quit");

    connect(quitAction, &QAction::triggered, this, [=] () {
        this -> close();
    });

    loadMoney(ARCHIVE_PATH);

    moneyLabel = new QLabel(QString("Your money: %1").arg(money));
    QFont ft;
    ft.setPointSize(12);
    moneyLabel -> setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::white);
    moneyLabel -> setPalette(pa);
    moneyLabel -> setParent(this);
    moneyLabel -> setFixedSize(200, 50);
    moneyLabel -> move(this -> width() * 0.1, this -> height() * 0.9);

    QString speed_string;
    speed_string = QString("Speed Level: %1").arg(speedLevel);
    speedLevelLabel = new QLabel;
    setLabel(speedLevelLabel, speed_string, 0.1, 0.2);
    QString hp_string;
    hp_string = QString("Hp Level: %1").arg(hpLevel);
    hpLevelLabel = new QLabel;
    setLabel(hpLevelLabel, hp_string, 0.4, 0.2);
    QString attack_string;
    attack_string = QString("Attack Level: %1").arg(attackLevel);
    attackLevelLabel = new QLabel;
    setLabel(attackLevelLabel, attack_string, 0.7, 0.2);
    QString relive_string;
    relive_string = QString("relive Level: %1").arg(reliveLevel);
    reliveLabel = new QLabel;
    setLabel(reliveLabel, relive_string, 0.1, 0.5);
    QString money_coefficient_string;
    money_coefficient_string = QString("Money Coefficient Level: %1").arg(moneyCoefficientLevel);
    moneyCoefficientLabel = new QLabel;
    setLabel(moneyCoefficientLabel, money_coefficient_string, 0.4, 0.5);

    QPushButton *backButton = new QPushButton;
    backButton -> setParent(this);
    backButton -> setText("Back");
    backButton -> setFixedSize(100, 50);
    backButton -> move(this -> width() * 0.9, this -> height() * 0.9);

    connect(backButton, &QPushButton::clicked, [=] () {
        this->hide();
        backButtonClicked();
    });

    QPushButton *speedButton = new QPushButton;
    speedButton -> setParent(this);
    speedButton -> setText("speed");
    speedButton -> setFixedSize(100, 50);
    speedButton -> move(this -> width() * 0.1, this -> height() * 0.3);

    connect(speedButton, &QPushButton::clicked, [=] () {
        buyItem(SPEED);
    });

    QPushButton *hpButton = new QPushButton;
    hpButton -> setParent(this);
    hpButton -> setText("hp");
    hpButton -> setFixedSize(100, 50);
    hpButton -> move(this -> width() * 0.4, this -> height() * 0.3);

    connect(hpButton, &QPushButton::clicked, [=] () {
        buyItem(HP);
    });

    QPushButton *attackButton = new QPushButton;
    attackButton -> setParent(this);
    attackButton -> setText("attack");
    attackButton -> setFixedSize(100, 50);
    attackButton -> move(this -> width() * 0.7, this -> height() * 0.3);

    connect(attackButton, &QPushButton::clicked, [=] () {
        buyItem(ATTACK);
    });

    QPushButton *reliveButton = new QPushButton;
    reliveButton -> setParent(this);
    reliveButton -> setText("revive");
    reliveButton -> setFixedSize(100, 50);
    reliveButton -> move(this -> width() * 0.1, this -> height() * 0.6);

    connect(reliveButton, &QPushButton::clicked, [=] () {
        buyItem(RELIVE);
    });

    QPushButton *coefficientButton = new QPushButton;
    coefficientButton -> setParent(this);
    coefficientButton -> setText("money coefficient");
    coefficientButton -> setFixedSize(100, 50);
    coefficientButton -> move(this -> width() * 0.4, this -> height() * 0.6);

    connect(coefficientButton, &QPushButton::clicked, [=] () {
        buyItem(MONEY_COEFFICIENT);
    });
};

ShopScene::~ShopScene() {
    delete moneyLabel;
    delete speedLevelLabel;
    delete hpLevelLabel;
    delete attackLevelLabel;
    delete reliveLabel;
    delete moneyCoefficientLabel;
}

void ShopScene::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    QPixmap background_pix;
    background_pix.load(":/images/indexBackground.jpg");
    painter.drawPixmap(0, 0, this -> width(), this -> height(), background_pix);
}

void ShopScene::loadMoney(const QString &filename) {
    //QString settings_path = QDir::homePath() + filename;
    QSettings settings(filename, QSettings::IniFormat);
    money = settings.value(QString("shop/money"), 10).toInt();
    speedLevel = settings.value(QString("shop/speed_up_level"), 1).toInt();
    hpLevel = settings.value(QString("shop/hp_up_level"), 1).toInt();
    attackLevel = settings.value(QString("shop/attack_up_level"), 1).toInt();
    reliveLevel = settings.value(QString("shop/relive_level"), 1).toInt();
    moneyCoefficientLevel = settings.value(QString("shop/money_coefficient_level"), 1).toInt();
//    moneyLabel -> setText(QString("Your money: %1"));
    qDebug() << settings.value("shop/money", 10).toInt();
}

void ShopScene::buyItem(itemType type) {
    QSettings settings(ARCHIVE_PATH, QSettings::IniFormat);
    if(type == SPEED) {
        int speed_level = settings.value(QString("shop/speed_up_level"), 1).toInt();
        bool if_max = speed_level >= MAX_SPEED_LEVEL;
        if (if_max) {
            QMessageBox::critical(nullptr, "MAX", "You have max level!");
            return;
        }
        else if(money < speed_level * 100) {
            qDebug() << "No money";
            QMessageBox::critical(nullptr, "NO MONEY", "You do not have enough money!");
        }
        else {
            settings.setValue("shop/speed_up_level", speed_level + 1);
            settings.setValue("shop/money", money - speed_level * 100);
            settings.sync();
            money -= speed_level * 100;
            moneyLabel -> setText(QString("Your money: %1").arg(money - speed_level * 100));
            speedLevelLabel -> setText(QString("Speed Level: %1").arg(++speedLevel));
        }
    }
    else if(type == HP) {
        int hp_level = settings.value(QString("shop/hp_up_level"), 1).toInt();
        bool if_max = hp_level >= MAX_HP_LEVEL;
        if (if_max) {
            QMessageBox::critical(nullptr, "MAX", "You have max level!");
            return;
        }
        else if(money < hp_level * 100) {
            qDebug() << "No money";
            QMessageBox::critical(nullptr, "NO MONEY", "You do not have enough money!");
        }
        else {
            settings.setValue("shop/hp_up_level", hp_level + 1);
            settings.setValue("shop/money", money - hp_level * 100);
            settings.sync();
            money -= hp_level * 100;
            moneyLabel -> setText(QString("Your money: %1").arg(money - hp_level * 100));
            hpLevelLabel -> setText(QString("Hp Level: %1").arg(++hpLevel));
        }
    }
    else if(type == ATTACK) {
        int attack_level = settings.value(QString("shop/attack_up_level"), 1).toInt();
        bool if_max = attack_level >= MAX_ATTACK_LEVEL;
        if (if_max) {
            QMessageBox::critical(nullptr, "MAX", "You have max level!");
            return;
        }
        else if(money < attack_level * 100) {
            qDebug() << "No money";
            QMessageBox::critical(nullptr, "NO MONEY", "You do not have enough money!");
        }
        else {
            settings.setValue("shop/attack_up_level", attack_level++);
            settings.setValue("shop/money", money - attack_level * 100);
            settings.sync();
            money -= attack_level * 100;
            moneyLabel -> setText(QString("Your money: %1").arg(money));
            attackLevelLabel -> setText(QString("Attack Level: %1").arg(++attackLevel));
        }
    }
    else if(type == RELIVE) {
        int relive_level = settings.value(QString("shop/relive_up_level"), 1).toInt();
        bool if_max = relive_level >= MAX_RELIVE_LEVEL;
        if (if_max) {
            QMessageBox::critical(nullptr, "MAX", "You have max level!");
            return;
        }
        else if(money < relive_level * 100) {
            qDebug() << "No money";
            QMessageBox::critical(nullptr, "NO MONEY", "You do not have enough money!");
        }
        else {
            settings.setValue("shop/relive_up_level", relive_level + 1);
            settings.setValue("shop/money", money - relive_level * 1000);
            settings.sync();
            money -= relive_level * 1000;
            moneyLabel -> setText(QString("Your money: %1").arg(money - relive_level * 100));
            reliveLabel -> setText(QString("relive Level: %1").arg(++reliveLevel));
        }
    }
    else if(type == MONEY_COEFFICIENT) {
        int coefficient_level = settings.value(QString("shop/money_coefficient_level"), 1).toInt();
        bool if_max = coefficient_level >= MAX_COEFFICIENT_LEVEL;
        if (if_max) {
            QMessageBox::critical(nullptr, "MAX", "You have max level!");
            return;
        }
        else if(money < coefficient_level * 100) {
            qDebug() << "No money";
            QMessageBox::critical(nullptr, "NO MONEY", "You do not have enough money!");
        }
        else {
            settings.setValue("shop/money_coefficient_level", coefficient_level + 1);
            settings.setValue("shop/money", money - coefficient_level * 100);
            settings.sync();
            money -= coefficient_level * 100;
            moneyLabel -> setText(QString("Your money: %1").arg(money - coefficient_level * 100));
            moneyCoefficientLabel -> setText(QString("Money Coefficient Level: %1").arg(++moneyCoefficientLevel));
        }
    }
}
