/*** 
 * @Author       : KnightZJ
 * @LastEditTime : 2022-02-17 20:22:30
 * @LastEditors  : KnightZJ
 * @Description  : poker header file
 */

#include <stdint.h>

typedef uint16_t Card;
typedef uint64_t CardsGroup;

typedef enum CardType {
    c_3,
    c_4,
    c_5,
    c_6,
    c_7,
    c_8,
    c_9,
    c_10,
    c_J,
    c_Q,
    c_K,
    c_A,
    c_2,
    c_BlackJoker,
    c_RedJoker
}CardType;

extern const Card cSingle[15];
extern const char* strCardType[15];
const char* strCardGroupType[15];

typedef enum CardsGroupType {
    cg_Invalid,
    cg_Single,
    cg_Double,
    cg_Three,
    cg_ThreeWithSingle,
    cg_ThreeWithDouble,
    cg_Bomb,
    cg_KingBomb,
    cg_FourWithSingles,
    cg_FourWithDoubles,
    cg_Sequence,
    cg_DoubleSequence,
    cg_Airplane,
    cg_AirplaneWithSingles,
    cg_AirplaneWithDoubles
}CardsGroupType;

/*** 
 * @description: count the number of a specified card in the given cards group
 * @param      {CardsGroup} the given cards group
 * @param      {CardType} type of the specified card
 * @return     the number of a specified card in a cards set
 */
int count_single_card(CardsGroup cardsgroup, CardType cardtype);

/*** 
 * @description: count all cards in the given cards group
 * @param      {CardsGroup} the given cards group
 * @return     the number of cards in the given cards group
 */
int count_all_cards(CardsGroup cardsgroup);




