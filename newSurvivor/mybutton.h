
#ifndef MYBUTTON_H
#define MYBUTTON_H


#include <QWidget>
#include <QPushButton>
#include <QPixmap>
#include "levelchoosescene.h"

class MyButton : public QPushButton
{
    Q_OBJECT
    friend class LevelChooseScene;
    int iconwidth;
    int iconheight;
    QPixmap buttonIcon;
public:
    // explicit MyButton(QPushButton *parent = nullptr);
    MyButton(QString normalImg);
    MyButton(QString normalImg, int iconWidth, int iconHeight);

    QString normalImgPath;

    void clickedUp();
    void clickedDown();

signals:

};

#endif // MYBUTTON_H

