
#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

//说明
//mobileItem继承自QGraphicsObject
//bullet 子弹 dropItem 掉落物 enemyItem 敌人
//mapItem 地图项 obstacle 障碍物
//以上均继承自mobileItem
//Player玩家项 Wave光剑攻击 继承自QGraphicsObject
//mainWindow主界面 levelchoosescene 关卡选择界面
//shopScene商店界面 GameScene 游戏界面
//以上均继承自QMainWindow
//GameController 游戏控制器 itemPool对象池
//继承自QObject
//Global.h 宏定义 全局
