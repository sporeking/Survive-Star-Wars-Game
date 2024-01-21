
#ifndef GLOBAL_H
#define GLOBAL_H

enum direction {NOMOVE, UP, DOWN, LEFT, RIGHT} ;

enum obstacleSet {Falcon, Ti, Xwings};

enum ObjectData {
    Type
};

enum ObjectType {
    obstacleType,
    playerType,
    mapType,
    enemyBulletType,
    playerBulletType,
    enemyType,
    dropItemType
};


enum itemType {
    SPEED,
    HP,
    ATTACK,
    RELIVE,
    MONEY_COEFFICIENT,

    IF_IN_GAME,
    GAMELEVEL,
    REMAINTIME,
    PLAYER_TYPE,
    PLAYER_LEVEL,
    PLAYER_MAX_HP,
    PLAYER_SPEED,
    PLAYER_HP
};

enum dropType {
    EXP_DROP,
    MONEY_DROP,
    HP_DROP
};

#define REMAIN_TIME 180

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define LEVEL_BUTTON_WIDTH 250
#define LEVEL_BUTTON_HEIGHT 100
#define STEPS 50
#define LUKE_WIDTH 79
#define LUKE_HEIGHT 183
#define HAN_WIDTH 93
#define HAN_HEIGHT 158
#define LUKE_HP 100
#define LUKE_SPEED 8
#define HAN_HP 140
#define HAN_SPEED 5
#define ARCHIVE_PATH "./archive.ini"
#define MAX_SPEED_LEVEL 5
#define MAX_HP_LEVEL 5
#define MAX_ATTACK_LEVEL 5
#define MAX_RELIVE_LEVEL 2
#define MAX_COEFFICIENT_LEVEL 5
#define OBSTACLE_WIDTH 50
#define ENEMY_HP_1 60
#define ENEMY_HP_2 50
#define ENEMY_HP_3 200
#define ENEMY_HP_BOSS 2000
#define ENEMY_ATTACK_1 1
#define ENEMY_ATTACK_2 1
#define ENEMY_ATTACK_3 3
#define ENEMY_ATTACK_BOSS 10
#define ENEMY_SPEED_1 4
#define ENEMY_SPEED_2 5
#define ENEMY_SPEED_3 6
#define ENEMY_SPEED_BOSS1 4
#define ENEMY_SPEED_BOSS 6
#define ENEMY_DAMAGE_RANGE_BOSS1 80
#define ENEMY_DAMAGE_RANGE_BOSS 60
#define SHOOT_FREQUENCY 2
#endif // GLOBAL_H
