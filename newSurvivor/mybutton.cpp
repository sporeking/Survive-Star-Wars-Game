
#include "mybutton.h"
#include <QString>
#include <QPainter>

MyButton::MyButton(QString normalImg) {
    this -> normalImgPath = normalImg;

    bool ret = buttonIcon.load(normalImgPath);
    if(!ret){

    }
    iconwidth = buttonIcon.width();
    iconheight = buttonIcon.height();

    this -> setFixedSize(iconwidth, iconheight);
    //设置不规则图片样式
    this -> setStyleSheet("QPushButton{border: 0px; background-color: transparent;}");

    this -> setIcon(buttonIcon);

    this -> setIconSize(QSize(iconwidth, iconheight));
}

MyButton::MyButton(QString normalImg, int iconWidth, int iconHeight) {
    this -> normalImgPath = normalImg;

    bool ret = buttonIcon.load(normalImgPath);
    if(!ret){

    }
    iconwidth = iconWidth;
    iconheight = iconHeight;

    this -> setFixedSize(iconwidth, iconheight);
    //设置不规则图片样式
    this -> setStyleSheet("QPushButton{border: 0px; background-color: transparent;}");

    this -> setIcon(buttonIcon);

    this -> setIconSize(QSize(iconwidth, iconheight));
}

void MyButton::clickedUp() {
    buttonIcon.load(normalImgPath);

    QPixmap temp(QSize(iconwidth, iconheight));
    temp.fill(Qt::transparent);

    QPainter p1(&temp);
    p1.setCompositionMode(QPainter::CompositionMode_Source);
    p1.drawPixmap(0, 0, buttonIcon);
    p1.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p1.fillRect(temp.rect(), QColor(255, 255, 255, 128));
    p1.end();
    buttonIcon = temp;

    this -> setFixedSize(iconwidth, iconheight);
    //设置不规则图片样式
    this -> setStyleSheet("QPushButton{border: 0px; background-color: transparent;}");

    this -> setIcon(buttonIcon);

    this -> setIconSize(QSize(iconwidth, iconheight));
}


void MyButton::clickedDown() {
    buttonIcon.load(normalImgPath);

    QPixmap temp(QSize(iconwidth, iconheight));
    temp.fill(Qt::transparent);

    QPainter p1(&temp);
    p1.setCompositionMode(QPainter::CompositionMode_Source);
    p1.drawPixmap(0, 0, buttonIcon);
    p1.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p1.fillRect(temp.rect(), QColor(255, 255, 255, 255));
    p1.end();
    buttonIcon = temp;

    this -> setFixedSize(iconwidth, iconheight);
    //设置不规则图片样式
    this -> setStyleSheet("QPushButton{border: 0px; background-color: transparent;}");

    this -> setIcon(buttonIcon);

    this -> setIconSize(QSize(iconwidth, iconheight));
}
