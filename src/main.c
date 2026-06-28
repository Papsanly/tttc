#include "str.c"
#include <stdio.h>

enum : uint8_t { CELL_EMPTY = 0, CELL_X = 1, CELL_O = 2 };

enum : uint8_t { PLAYER_X = 1, PLAYER_O = 2 };

const uint8_t WINNING_CELL_SETS[8][3] = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6},
};

typedef struct {
    uint8_t current_player;
    uint8_t board[9];
} Game;

Game game_new() { return (Game){.current_player = PLAYER_X}; }

void game_start() { printf("Welcome to tik-tak-toe terminal game (c version)!\n"); }

char cell_to_char(uint8_t cell) {
    if (cell == CELL_X) {
        return 'x';
    } else {
        return 'o';
    }
}

void board_print(uint8_t board[9]) {
    printf("\n");
    for (uint8_t row = 0; row < 3; row++) {
        for (uint8_t col = 0; col < 3; col++) {
            uint8_t idx = col + 3 * row;
            uint8_t cell = board[idx];
            if (cell == CELL_EMPTY) {
                printf("|%d", idx);
            } else {
                printf("|%c", cell_to_char(cell));
            }
        }
        printf("|\n");
    }
    printf("\n");
}

void game_make_turn(Game *game) {
    printf("Player %c turn\nEnter cell number: ", cell_to_char(game->current_player));
    while (1) {
        char cell_str[255] = {};
        fgets(cell_str, sizeof(cell_str), stdin);
        char *cell_str_trimmed = str_trim(cell_str);
        uint8_t err = 0;
        int64_t cell = str_to_int(cell_str_trimmed, &err);
        if (err) {
            if (err == STR_TO_INT_ERR) {
                printf("Error while parsing cell index\n");
            } else {
                printf("Error: cell index is too large\n");
            }
            printf("Try again: ");
            continue;
        }
        if (cell < 0 || cell >= 9) {
            printf("Error: cell index out of bounds\n");
            printf("Try again: ");
            continue;
        }
        if (game->board[cell] != CELL_EMPTY) {
            printf("Error: cell is already occupied\n");
            printf("Try again: ");
            continue;
        }
        game->board[cell] = game->current_player;
        break;
    }
}

uint8_t game_is_won(Game *game) {
    int sets_count = sizeof(WINNING_CELL_SETS) / sizeof(WINNING_CELL_SETS[0]);
    int set_size = sizeof(WINNING_CELL_SETS[0]) / sizeof(WINNING_CELL_SETS[0][0]);
    for (int set_idx = 0; set_idx < sets_count; set_idx++) {
        int condition = 1;
        for (int idx = 0; idx < set_size; idx++) {
            int cell_idx = WINNING_CELL_SETS[set_idx][idx];
            uint8_t cell = game->board[cell_idx];
            if (cell != game->current_player) {
                condition = 0;
                break;
            }
        }
        if (condition) {
            return 1;
        }
    }
    return 0;
}

void game_won(uint8_t winner) { printf("Congratulations! Player %c won!\n", cell_to_char(winner)); }

void game_draw() { printf("Draw!\n"); }

uint8_t board_is_full(uint8_t board[9]) {
    for (int cell_idx = 0; cell_idx < 9; cell_idx++) {
        uint8_t cell = board[cell_idx];
        if (cell == CELL_EMPTY) {
            return 0;
        }
    }
    return 1;
}

uint8_t player_switch(uint8_t player) {
    if (player == PLAYER_X) {
        return PLAYER_O;
    } else {
        return PLAYER_X;
    }
}

void game_run(Game *game) {
    game_start();
    while (1) {
        board_print(game->board);
        game_make_turn(game);
        if (game_is_won(game)) {
            board_print(game->board);
            game_won(game->current_player);
            break;
        }
        if (board_is_full(game->board)) {
            board_print(game->board);
            game_draw();
            break;
        }
        game->current_player = player_switch(game->current_player);
    }
}

#ifdef TEST

int main() { test_str_to_int(); }

#else

int main() {
    Game game = game_new();
    game_run(&game);
}

#endif
