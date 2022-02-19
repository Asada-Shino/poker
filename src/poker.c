/*
 * @Author       : KnightZJ
 * @LastEditTime : 2022-02-19 14:02:52
 * @LastEditors  : KnightZJ
 * @Description  : poker source file
 */

#include "poker.h"
#include <assert.h>
#include <stdlib.h>

const Card cSingle[15] = {
    0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000
};

const CardsGroup cgSingleCardMask[15] = {
    0x1000100010001, 0x2000200020002, 0x4000400040004, 0x8000800080008,
    0x10001000100010, 0x20002000200020, 0x40004000400040, 0x80008000800080,
    0x100010001000100, 0x200020002000200, 0x400040004000400, 0x800080008000800,
    0x1000100010001000, 0x2000, 0x4000
};

const CardsGroup cgCardNumMask[5] = {
    0x0, 0x1, 0x10001, 0x100010001, 0x1000100010001
};

const CardsGroup cgCardsRowMask[4] = {
    0xffff, 0xffff0000, 0xffff00000000, 0xffff000000000000
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

const char* strJudgeRes[5] = {
    "牌型不一致",
    "非法牌型",
    "压得过",
    "压不过",
    "未知错误"
};

int count_single_card(CardsGroup cardsgroup, CardType cardtype) {
    return count_all_cards(cardsgroup & cgSingleCardMask[cardtype]);
}

int count_all_cards(CardsGroup cardsgroup) {
    int res = 0;
    while(cardsgroup) {
        cardsgroup &= (cardsgroup - 1);
        ++res;
    }
    return res;
}

int can_take_card(CardsGroup cardsgroup, CardType cardtype, int num) {
    assert(num > 0 && num <= 4);
    return count_single_card(cardsgroup, cardtype) >= num;
}

int take_card(CardsGroup* cardsgroup, CardType cardtype, int num) {
    if(!can_take_card(*cardsgroup, cardtype, num))
        return 0;
    *cardsgroup = (*cardsgroup & ~cgSingleCardMask[cardtype]) | 
                (cgCardNumMask[count_single_card(*cardsgroup, cardtype) - num] << cardtype);
    return 1;
}

int can_add_card(CardsGroup cardsgroup, CardType cardtype, int num) {
    assert(num > 0 && num <= 4);
    if(cardtype >= c_BlackJoker)
        return num <= 1 && count_single_card(cardsgroup, cardtype) < num;
    return count_single_card(cardsgroup, cardtype) <= 4 - num;
}

int add_card(CardsGroup* cardsgroup, CardType cardtype, int num) {
    if(!can_add_card(*cardsgroup, cardtype, num))
        return 0;
    *cardsgroup = (*cardsgroup & ~cgSingleCardMask[cardtype]) | 
                (cgCardNumMask[count_single_card(*cardsgroup, cardtype) + num] << cardtype);
    return 1;
}

int can_move_card(CardsGroup dest, CardsGroup source, CardType cardtype, int num) {
    return can_take_card(source, cardtype, num) && can_add_card(dest, cardtype, num);
}

int move_card(CardsGroup* dest, CardsGroup* source, CardType cardtype, int num) {
    if(!can_move_card(*dest, *source, cardtype, num))
        return 0;
    return take_card(source, cardtype, num) && add_card(dest, cardtype, num);
}

int check_sequence(CardsGroup cards, int len) {
    CardsGroup test = (1<<len)-1;
    while(test <= 0xfffULL) {
        if((test & cards) == test)
            return 1;
        test <<= 1;
    }
    return 0;
}

CardsGroupInfo get_cards_info(CardsGroup cards) {
    CardsGroup singles = (cards & cgCardsRowMask[0]), doubles = (cards & cgCardsRowMask[1]) >> 16,
                threes = (cards & cgCardsRowMask[2]) >> 32, fours = (cards & cgCardsRowMask[3]) >> 48;
    singles ^= doubles, doubles ^= threes, threes ^= fours;
    int cnt = count_all_cards(cards), cntSingle = count_all_cards(singles),
        cntDouble = count_all_cards(doubles), cntThree = count_all_cards(threes);
    if(cnt >= 5 && cnt <= 13 && check_sequence(singles, cnt))
        return (CardsGroupInfo){.type = cg_Sequence, .sequence_cnt = cnt, .sequence = singles};
    if(cnt % 2 == 0 && cnt >= 6 && cnt <= 20 && check_sequence(doubles, cnt / 2))
        return (CardsGroupInfo){.type = cg_DoubleSequence, .sequence_cnt = cnt / 2, .sequence = doubles};
    if(cnt % 3 == 0 && cnt >= 6 && cnt <= 18 && check_sequence(threes, cnt / 3))
        return (CardsGroupInfo){.type = cg_Airplane, .sequence_cnt = cnt / 3, .sequence = threes};
    switch(cnt) {
        case 1:
            return (CardsGroupInfo){.type = cg_Single, .sequence_cnt = 1, .sequence = singles};
        case 2:
            if(doubles) return (CardsGroupInfo){.type = cg_Double, .sequence_cnt = 1, .sequence = doubles};
            if((singles & cSingle[c_BlackJoker]) && (singles & cSingle[c_RedJoker]))
                return (CardsGroupInfo){.type = cg_KingBomb};
            break;
        case 3:
            if(threes) return (CardsGroupInfo){.type = cg_Three, .sequence_cnt = 1, .sequence = threes};
            break;
        case 4:
            if(fours) return (CardsGroupInfo){.type = cg_Bomb, .sequence_cnt = 1, .sequence = fours};
            if(threes && singles)
                return (CardsGroupInfo){.type = cg_ThreeWithSingle, .sequence_cnt = 1, .sequence = threes};
            break;
        case 5:
            if(threes && doubles)
                return (CardsGroupInfo){.type = cg_ThreeWithDouble, .sequence_cnt = 1, .sequence = threes};
            break;
        case 6:
            if(fours && (cntDouble * 2 + cntSingle == 2))
                return (CardsGroupInfo){.type = cg_FourWithSingles, .sequence_cnt = 1, .sequence = fours};
            break;
        case 8:
            if(fours && (cntDouble == 2))
                return (CardsGroupInfo){.type = cg_FourWithDoubles, .sequence_cnt = 1, .sequence = fours};
            if(check_sequence(threes, 2) && cntDouble * 2 + cntSingle == 2)
                return (CardsGroupInfo){.type = cg_AirplaneWithSingles, .sequence_cnt = 2, .sequence = threes};
            break;
        case 10:
            if(check_sequence(threes, 2) && cntDouble == 2)
                return (CardsGroupInfo){.type = cg_AirplaneWithDoubles, .sequence_cnt = 2, .sequence = threes};
            break;
        case 12:
            if(check_sequence(threes, 3) && !check_sequence(threes, 4)&& (cntThree - 3)*3 + cntDouble * 2 + cntSingle == 3)
                return (CardsGroupInfo){.type = cg_AirplaneWithSingles, .sequence_cnt = 3, .sequence = threes};
            break;
        case 15:
            if(check_sequence(threes, 3) && cntDouble == 3)
                return (CardsGroupInfo){.type = cg_AirplaneWithDoubles, .sequence_cnt = 3, .sequence = threes};
            break;
        case 16:
            if(check_sequence(threes, 4) && !check_sequence(threes, 5) && (cntThree - 4)*3 + cntDouble * 2 + cntSingle == 4)
                return (CardsGroupInfo){.type = cg_AirplaneWithSingles, .sequence_cnt = 4, .sequence = threes};
            break;
        case 20:
            if(check_sequence(threes, 4) && doubles == 4)
                return (CardsGroupInfo){.type = cg_AirplaneWithDoubles, .sequence_cnt = 4, .sequence = threes};
            if(check_sequence(threes, 5) && !check_sequence(threes, 6) && (cntThree - 5)*3 + cntDouble * 2 + cntSingle == 5)
                return (CardsGroupInfo){.type = cg_AirplaneWithDoubles, .sequence_cnt = 5, .sequence = threes};;
            break;
    }
    return (CardsGroupInfo){.type = cg_Invalid};
}

JudgeRes judge(CardsGroup last, CardsGroup challenger) {
    CardsGroupInfo last_info = get_cards_info(last), challenger_info = get_cards_info(challenger);
    if(last_info.type == cg_Invalid)
        return jr_Unexpected;
    if(challenger_info.type == cg_Invalid)
        return jr_InvalidInput;
    if(last_info.type == cg_KingBomb)
        return jr_Smaller;
    if(challenger_info.type == cg_KingBomb)
        return jr_Bigger;
    if(challenger_info.type == cg_Bomb) {
        if(last_info.type == cg_Bomb)
            return challenger_info.sequence > last_info.sequence ? jr_Bigger : jr_Smaller;
        return jr_Bigger;
    }
    if(challenger_info.type != last_info.type)
        return jr_WrongMatch;
    if(challenger_info.sequence_cnt != last_info.sequence_cnt)
        return jr_WrongMatch;
    return challenger_info.sequence > last_info.sequence ? jr_Bigger : jr_Smaller; 
}

int shuffle(Table* table, long seed) {
    srand(seed);
    char a[54];
    for(int i = 0; i < 54; ++i)
        a[i] = i;
    for(int i = 0; i < 54; ++i) {
        int random = i + rand()%(54-i);
        int num = a[i];
        a[i] = a[random];
        a[random] = num;
    }
    table->landlord_cards = table->cards[0] = table->cards[1] = table->cards[2] = 0;
    for(int i = 0; i < 51; ++i)
        add_card(&table->cards[i/17], a[i] >= 52 ? a[i] - 52 + 13 : a[i] / 4, 1);
    for(int i = 51; i < 54; ++i)
        add_card(&table->landlord_cards, a[i] >= 52 ? a[i] - 52 + 13 : a[i] / 4, 1);
    return 1;
}

