#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "state.h"

int main(void)
{
    struct state s;

    srand(time(NULL));
    init_state(&s);

    while (!has_won(&s)) {
        print_state(&s);
    }

    return 0;
}
