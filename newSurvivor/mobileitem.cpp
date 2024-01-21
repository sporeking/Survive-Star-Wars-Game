
#include "mobileitem.h"
#include <QPainter>

mobileItem::mobileItem(Player &mainPlayer):mainPlayer(mainPlayer), speed(mainPlayer.getSpeed()), pixmapNum(0)  {
    // setFlag(ItemHasNoContents, true);
    //itemNum++;
}

bool mobileItem::W = false;
bool mobileItem::A = false;
bool mobileItem::S = false;
bool mobileItem::D = false;
bool mobileItem::W_block = false;
bool mobileItem::A_block = false;
bool mobileItem::S_block = false;
bool mobileItem::D_block = false;

void mobileItem::advance(int step) {
    if (step == 0) {
        // 在第一个阶段更新数据
    } else {
        // 在第二个阶段更新绘制
        update();
    }
}

void mobileItem::moveAll() {
//    mobileItem::h_speed = A != D ? 0.7 * speed : speed;
//    mobileItem::v_speed = W != S ? 0.7 * speed : speed;
    if(W && !W_block) {
        qreal newY;
        if(A != D)
            newY = scenePos().ry() + 0.7 * speed;
        else
            newY = scenePos().ry() + speed;
        setPos(pos().x(), newY);
    }
    if (S && !S_block) {
        qreal newY;
        if(A != D)
            newY = pos().y() - 0.7 * speed;
        else
            newY = pos().y() - speed;
        setPos(pos().x(), newY);
    }
    if (A && !A_block) {
        qreal newX;
        if(W != S)
            newX = pos().x() + 0.7 * speed;
        else
            newX = pos().x() + speed;
        setPos(newX, pos().y());
    }
    if (D && !D_block) {
        qreal newX;
        if(W != S)
            newX = pos().x() - 0.7 * speed;
        else
            newX = pos().x() - speed;
        setPos(newX, pos().y());
    }
}
