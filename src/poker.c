/*
 * @Author       : KnightZJ
 * @LastEditTime : 2022-02-17 21:48:39
 * @LastEditors  : KnightZJ
 * @Description  : poker source file
 */

#include "poker.h"

const Card cSingle[15] = {
    0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000
};

const CardsGroup cgCardMask[15] = {
    0x1000100010001, 0x2000200020002, 0x4000400040004, 0x8000800080008,
    0x10001000100010, 0x20002000200020, 0x40004000400040, 0x80008000800080,
    0x100010001000100, 0x200020002000200, 0x400040004000400, 0x800080008000800,
    0x1000100010001000, 0x2000, 0x4000
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

/*** 
 * @description: count the number of a specified card in the given cards group
 * @param      {CardsGroup} the given cards group
 * @param      {CardType} type of the specified card
 * @return     the number of a specified card in a cards set
 */
int count_single_card(CardsGroup cardsgroup, CardType cardtype) {
    return count_all_cards(cardsgroup & cgCardMask[cardtype]);
}

/*** 
 * @description: count all cards in the given cards group
 * @param      {CardsGroup} the given cards group
 * @return     the number of cards in the given cards group
 */
int count_all_cards(CardsGroup cardsgroup) {
    int res = 0;
    while(cardsgroup) {
        cardsgroup &= (cardsgroup - 1);
        ++res;
    }
    return res;
}