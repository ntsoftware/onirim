#pragma once

#define HAND_SIZE       5
#define DECK_SIZE       76
#define LIMBO_SIZE      76
#define LABYRINTH_SIZE  76

enum symbol {
    NONE        = 0,
    SUN         = 1,
    MOON        = 2,
    KEY         = 3,
    DOOR        = 4,
    NIGHTMARE   = 5,
};

enum color {
    RED         = 0,
    BLUE        = 1,
    GREEN       = 2,
    BROWN       = 3,
};

struct card {
    enum symbol symbol  :3;
    enum color color    :2;
};

struct state {
    char open_doors[4];
    struct card hand[HAND_SIZE];
    struct card deck[DECK_SIZE];
    struct card limbo[LIMBO_SIZE];
    struct card labyrinth[LABYRINTH_SIZE];
};

void init_state(struct state *s);
struct card draw_card(struct state *s);
void put_in_hand(struct state *s, struct card);
void put_in_limbo(struct state *s, struct card c);
void shuffle_limbo(struct state *s);
int can_play_card(struct state *s, struct card c);
void play_card(struct state *s, struct card c);
void open_door(struct state *s, enum color color);
int has_won(struct state *s);
