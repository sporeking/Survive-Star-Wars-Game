
#include "dropitem.h"
#include <QPainter>

//DropItem::DropItem(QObject *parent)
//    : QObject{parent}
//{

//}

DropItem::DropItem(Player &mainPlayer, dropType type): mobileItem(mainPlayer), type(type) {
    if(type == EXP_DROP) {
        dropPixmap = QPixmap(":/images/expDrop.png");
    }
    else if(type == MONEY_DROP) {
        dropPixmap = QPixmap(":/images/dropMoney.png");
    }
    else {
        dropPixmap = QPixmap(":/images/dropHp.png");
    }
    setData(ObjectData::Type, ObjectType::dropItemType);
    setZValue(2);
}

QRectF DropItem::boundingRect() const
{
    return QRectF(-dropPixmap.width() / 2, -dropPixmap.height() / 2, dropPixmap.width(), dropPixmap.height());
}

QPainterPath DropItem::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect().center(), 100, 100);
    return path;
}

void DropItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter -> drawPixmap(0, 0, dropPixmap);
}

void DropItem::advance(int step) {
    if(step == 0) {
        return;
    }
    moveAll();
}
