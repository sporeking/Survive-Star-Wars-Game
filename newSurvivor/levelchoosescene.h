
#ifndef LEVELCHOOSESCENE_H
#define LEVELCHOOSESCENE_H


#include <QMainWindow>
#include "gamescene.h"

class LevelChooseScene : public QMainWindow
{
    Q_OBJECT
    friend class MainWindow;
    int roleNum;
    int levelNum;
    GameScene *game = nullptr;
public:
    explicit LevelChooseScene(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);

signals:
    void backButtonClicked();
    void transitReturnMain();
};

#endif // LEVELCHOOSESCENE_H

