#ifndef SNAKE_GAME_GAME_SETTINGS_H
#define SNAKE_GAME_GAME_SETTINGS_H
#include "ansi_codes.h"


namespace Game {
    enum Direction {up, down, left, right};
    constexpr int WIDTH {100};
    constexpr int HEIGHT {25};
    constexpr int GAME_SPEED_MS {200};
    constexpr short SNAKE_INITIAL_LENGTH {3};

    struct Symbols {
        char boundary {'*'};
        std::string_view boundary_color {ANSI_CODES::Color::bold_blue};

        char snake {'#'};
        std::string_view snake_color {ANSI_CODES::Color::bold_green};

        char food {'@'};
        std::string_view food_color {ANSI_CODES::Color::bold_white};

        char field {' '};
    };

    constexpr Symbols symbols {};

    struct Point {
        int x {0};
        int y {0};
    };

    struct State {
        bool is_game_alive {true};
        int score {0};
    };
}


#endif //SNAKE_GAME_GAME_SETTINGS_H