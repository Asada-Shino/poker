/*
 * @Author       : KnightZJ
 * @LastEditTime : 2022-02-17 20:21:39
 * @LastEditors  : KnightZJ
 * @Description  : poker source file
 */

#include "poker.h"

const Card cSingle[15] = {
    0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000
};

const char* strCardType[15] = {
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "10",
    "J",
    "Q",
    "K",
    "A",
    "2",
    "小王",
    "大王"
};

const char* strCardGroupType[15] = {
    "非法牌型❌",
    "单张",
    "对子",
    "三张",
    "三带一",
    "三带一对",
    "炸弹💣",
    "王炸🚀",
    "四带二",
    "四带两对",
    "顺子",
    "连对",
    "飞机✈",
    "飞机✈带单",
    "飞机✈带双"
};