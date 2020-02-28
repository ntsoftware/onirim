#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "state.h"

static const struct card CARD_NONE = {NONE};
static const struct card CARD_NIGHTMARE = {NIGHTMARE};

static int count_cards(struct card *cards, int size)
{
    int i;
    for (i = 0; i < size; ++i) {
        if (cards[i].symbol == NONE)
            break;
    }
    return i;
}

static void swap_cards(struct card *cards, int i, int j)
{
    const struct card c = cards[j];
    cards[i] = cards[j];
    cards[j] = c;
}

static void pack_cards(struct card *cards, int size)
{
    int i, j;
    for(i = 0; i < size; ++i) {
        if (cards[i].symbol == NONE) {
            for (j = i + 1; j < size; ++j) {
                if (cards[j].symbol != NONE) {
                    swap_cards(cards, i, j);
                    break;
                }
            }
        }
    }
}

static void shuffle_cards(struct card *cards, int size)
{
    const int n = count_cards(cards, size);
    if (n > 0) {
        int i;
        for (i = 0; i < 100; ++i) {
            const int j = (n - 1) * rand() / RAND_MAX;
            const int k = (n - 1) * rand() / RAND_MAX;
            swap_cards(cards, j, k);
        }
    }
}

void init_state(struct state *s)
{
    static const int num_cards[4][4] = {
      /* SUN    MOON    KEY     DOOR */
        {9,     4,      3,      2},     /* RED */
        {8,     4,      3,      2},     /* BLUE */
        {7,     4,      3,      2},     /* GREEN */
        {6,     4,      3,      2},     /* BROWN */
    };
    struct card *ptr = s->deck;
    int i, j, k;

    memset(s, 0, sizeof(*s));

    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j) {
            const struct card c = {1 + j, i};
            const int n = num_cards[i][j];
            for (k = 0; k < n; ++k)
                *ptr++ = c;
        }
    }

    for (i = 0; i < 10; ++i)
        *ptr++ = CARD_NIGHTMARE;

    shuffle_cards(s->deck, DECK_SIZE);
}

static struct card draw_card_at(struct state *s, int i)
{
    const int n = count_cards(s->deck, DECK_SIZE);
    if (i < n) {
        const struct card c = s->deck[i];
        s->deck[i] = CARD_NONE;
        pack_cards(s->deck, DECK_SIZE);
        return c;
    } else {
        return CARD_NONE;
    }
}

struct card draw_card(struct state *s)
{
    return draw_card_at(s, 0);
}

struct card find_door(struct state *s, enum color color)
{

}

void put_in_hand(struct state *s, struct card c)
{
    int i;
    for (i = 0; i < HAND_SIZE; ++i) {
        if (s->hand[i].symbol == NONE) {
            s->hand[i] = c;
            return;
        }
    }
}

void put_in_limbo(struct state *s, struct card c)
{
    const int n = count_cards(s->limbo, LIMBO_SIZE);
    s->limbo[n] = c;
}

void shuffle_limbo(struct state *s)
{
    const int n_deck = count_cards(s->deck, DECK_SIZE);
    const int n_limbo = count_cards(s->limbo, LIMBO_SIZE);
    if (n_limbo > 0) {
        int i;
        for (i = 0; i < n_limbo; ++i) {
            s->deck[n_deck + i] = s->limbo[i];
            s->limbo[i] = CARD_NONE;
        }
        shuffle_cards(s->deck, DECK_SIZE);
    }
}

int can_play_card(struct state *s, struct card c)
{
    const int n = count_cards(s->labyrinth, LABYRINTH_SIZE);
    if (n > 0)
        return s->labyrinth[n - 1].symbol != c.symbol;
    else
        return 1;
}

void play_card(struct state *s, struct card c)
{
    const int n = count_cards(s->labyrinth, LABYRINTH_SIZE);
    s->labyrinth[n] = c;
}

void open_door(struct state *s, enum color color)
{
    const int n = count_cards(s->deck, DECK_SIZE);
    int i;
    for (i = 0; i < n; ++i) {
        const struct card c = s->deck[i];
        if (c.symbol == DOOR && c.color == color) {
            s->open_doors[color]++;
            s->deck[i] = CARD_NONE;
            pack_cards(s->deck, DECK_SIZE);
            shuffle_cards(s->deck, DECK_SIZE);
            return;
        }
    }
}

int has_won(struct state *s)
{
    int i;
    for (i = 0; i < 4; ++i) {
        if (s->open_doors[i] < 2)
            return 0;
    }
    return 1;
}
