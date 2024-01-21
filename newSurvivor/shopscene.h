
#ifndef SHOPSCENE_H
#define SHOPSCENE_H


#include <QMainWindow>
#include <QLabel>
#include "Global.h"

class ShopScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit ShopScene(QWidget *parent = nullptr);
    ~ShopScene();
    void paintEvent(QPaintEvent *);
    void loadMoney(const QString& filename);
    void buyItem(itemType type);

private:
    int money;
    int speedLevel;
    int hpLevel;
    int attackLevel;
    int reliveLevel;
    int moneyCoefficientLevel;
    QLabel *moneyLabel;
    QLabel *speedLevelLabel;
    QLabel *hpLevelLabel;
    QLabel *attackLevelLabel;
    QLabel *reliveLabel;
    QLabel *moneyCoefficientLabel;
    void setLabel(QLabel *label, QString &type_level, qreal wid, qreal hei);

signals:
    void backButtonClicked();
};

#endif // SHOPSCENE_H
