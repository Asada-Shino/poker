/*** 
 * @Author       : KnightZJ
 * @LastEditTime : 2022-02-19 13:58:33
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

typedef enum JudgeRes {
    jr_WrongMatch,
    jr_InvalidInput,
    jr_Bigger,
    jr_Smaller,
    jr_Unexpected
}JudgeRes;

typedef struct CardsGroupInfo {
    CardsGroupType type;
    int sequence_cnt;
    CardsGroup sequence;
}CardsGroupInfo;

typedef struct Table {
    CardsGroup cards[3];
    CardsGroup landlord_cards;
    int landlord_id;
    CardsGroup history[54];
    int steps;
    int turn;
    long seed;
}Table;

extern const Card cSingle[15];
extern const CardsGroup cgSingleCardMask[15];
extern const CardsGroup cgCardNumMask[5];
extern const CardsGroup cgCardsRowMask[4];
extern const char* strCardType[15];
extern const char* strCardGroupType[15];
extern const char* strJudgeRes[5];
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

/*** 
 * @description: judge if we can take {num} specified card(s) from the given cards group
 * @param      {CardsGroup} the given cards group
 * @param      {CardType} type of the specified card
 * @param      {int} the number of the cards to take
 * @return     {int} return none zero value if the card(s) can be successfully taken
 */
int can_take_card(CardsGroup cardsgroup, CardType cardtype, int num);

/*** 
 * @description: take {num} specified card(s) from the given cards group
 * @param      {CardsGroup*} the given cards group
 * @param      {CardType} type of the specified card
 * @param      {int} the number of the cards to take
 * @return     {int} return none zero value if the card(s) are successfully taken
 */
int take_card(CardsGroup* cardsgroup, CardType cardtype, int num);

/*** 
 * @description: judge if we can add {num} specified card(s) from the given cards group
 * @param      {CardsGroup} the given cards group
 * @param      {CardType} type of the specified card
 * @param      {int} the number of the cards to add
 * @return     {int} return none zero value if the card(s) can be successfully added
 */
int can_add_card(CardsGroup cardsgroup, CardType cardtype, int num);

/*** 
 * @description: add {num} specified card(s) from the given cards group
 * @param      {CardsGroup*} the given cards group
 * @param      {CardType} type of the specified card
 * @param      {int} the number of the cards to add
 * @return     {int} return none zero value if the card(s) are successfully added
 */
int add_card(CardsGroup* cardsgroup, CardType cardtype, int num);

/*** 
 * @description: judge if we can move {num} specified card(s) from source to dest
 * @param      {CardsGroup} dest
 * @param      {CardsGroup} source
 * @param      {CardType} cardtype
 * @param      {int} the number of the cards to move
 * @return     {int} return none zero value if the card(s) can be successfully moved
 */
int can_move_card(CardsGroup dest, CardsGroup source, CardType cardtype, int num);

/*** 
 * @description: move {num} specified card(s) from source to dest
 * @param      {CardsGroup*} dest
 * @param      {CardsGroup*} source
 * @param      {CardType} cardtype
 * @param      {int} the number of the cards to move
 * @return     {int} return none zero value if the card(s) are successfully moved
 */
int move_card(CardsGroup* dest, CardsGroup* source, CardType cardtype, int num);

/*** 
 * @description: get the cardstype and max sequence (if it has)
 * @param      {CardsGroup} cards
 * @return     {CardsGroupInfo}
 */
CardsGroupInfo get_cards_info(CardsGroup cards);

/*** 
 * @description: judge if {challenger} is bigger than {last}
 * @param      {CardsGroup} last
 * @param      {CardsGroup} challenger
 * @return     {JudgeRes} return Bigger if {challenger} is valid and really bigger than {last}
 */
JudgeRes judge(CardsGroup last, CardsGroup challenger);

/*** 
 * @description: generate player's cards
 * @param      {Table*} table
 * @param      {long} seed, used by srand
 * @return     {*} return none zero if successfully shuffled
 */
int shuffle(Table* table, long seed);
