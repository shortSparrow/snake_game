#include <iostream>
#include <thread>
#include <chrono>
#include <list>
#include <vector>
#include <mutex>
#include <random>
#include "game_settings.h"

#ifdef _WIN32
#include <windows.h>
#include <conio.h> // Для _kbhit() та _getch()
#endif



class SnakeGame {
    std::vector<std::vector<char>> board {Game::HEIGHT, std::vector<char>(Game::WIDTH, Game::symbols.field)};
    Game::State state {};
    Game::Point head {};
    Game::Point old_tail {};
    Game::Point food {};
    Game::Direction direction {};
    Game::Direction direction_candidate {};
    std::mutex state_mutex;
    std::thread thread {};
    bool is_loop_started {false};
    std::list<Game::Point> snake_body {};
    Game::Point score_position {3, Game::HEIGHT + 3};

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 engine{seed};

    /**
     * When we want to generate new food we must check all empty fields in board (avoid boundary and snake body)
     * and select random place for food.
     * This solution is not the best, because we loop throw all board and has teme complexity O(n), but I can't
     * figure out better solution for now. Maybe try to find out solution with time complexity O(log n) at least
     */
    Game::Point get_food_position() {
        std::vector<Game::Point> free_space_on_board {};

        // set free space to define available space for nex food
        for (auto i {1}; i<Game::HEIGHT-1; i++) { // start from 1 and Game::HEIGHT-1 because we have boundaries from top and bottom
            for (auto j{1}; j<Game::WIDTH-1; j++) {
                if (board[i][j] == Game::symbols.field) {
                    free_space_on_board.push_back(Game::Point{j,i});
                }
            }
        }

        const int last_index {static_cast<int>(free_space_on_board.size()) - 1};
        const int index = std::uniform_int_distribution{0, last_index}(engine);
        const auto point = free_space_on_board[index];

        return point;
    }

    void end_game() {
        state.is_game_alive = false;
    }

    void set_food_on_board(const Game::Point food_position) {
        board[food_position.y][food_position.x] = Game::symbols.food;
        food = food_position;
    }

    void init() {
        // create boundaries
        for (int i {0}; i < Game::WIDTH; ++i) {
            board[0][i] = Game::symbols.boundary;
            board[Game::HEIGHT-1][i] = Game::symbols.boundary;
        }
        for (int i {0}; i < Game::HEIGHT; ++i) {
            board[i][0] = Game::symbols.boundary;
            board[i][Game::WIDTH-1] = Game::symbols.boundary;
        }

        // set initial position
        constexpr int center_y {Game::HEIGHT/2};
        constexpr int center_x {Game::WIDTH/2};
        for (short i {0}; i< Game::SNAKE_INITIAL_LENGTH; i++) {
            const int x_position {center_x + i};
            board[center_y][x_position] = Game::symbols.snake;
            snake_body.push_front({x_position, center_y});
        }
        head = snake_body.front();

        // set food
        const auto food_position = get_food_position();
        set_food_on_board(food_position);
    }

    void apply_direction() {
        std::scoped_lock l(state_mutex);
        if (direction_candidate == direction || is_direction_opposite(direction_candidate)) {
            return;
        }
        direction = direction_candidate;
    }

    void make_move() {
        Game::Point next_head {};

        switch (direction) {
        case Game::down:
            next_head = {head.x, head.y + 1};
            break;
        case Game::up:
            next_head = {head.x, head.y - 1};
            break;
        case Game::left:
            next_head = {head.x - 1, head.y};
            break;
        case Game::right:
            next_head = {head.x + 1, head.y};
            break;
        }

        const char next_head_value {board[next_head.y][next_head.x]};
        if (next_head_value == Game::symbols.boundary || next_head_value == Game::symbols.snake) {
            end_game();
            return;
        } else {
            head = next_head;
            board[head.y][head.x] = Game::symbols.snake;
            snake_body.push_front({head.x, head.y});
            old_tail = snake_body.back();
            board[old_tail.y][old_tail.x] = Game::symbols.field;
            snake_body.pop_back();
        }

        if (next_head_value == Game::symbols.food) {
            // feed the snake
            board[old_tail.y][old_tail.x] = Game::symbols.field;
            snake_body.push_back(old_tail);

            // set food
            const auto new_food_position = get_food_position();
            set_food_on_board(new_food_position);

            state.score++;
        }

        print_score();
    }

    void run_loop(int ms) {
        if (is_loop_started == false) {
            thread = std::thread([this, ms]{
                while (state.is_game_alive) {
                    apply_direction();
                    make_move();
                    print_changes();
                    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
                }
            });
            is_loop_started = true;
        }
    }

    bool is_direction_opposite(const Game::Direction dir) const {
        auto const is_vertical_opposite {(direction == Game::down && dir == Game::up) || (direction == Game::up && dir == Game::down)};
        auto const is_horizontal_opposite {(direction == Game::left && dir == Game::right) || (direction == Game::right && dir == Game::left)};

        return is_vertical_opposite || is_horizontal_opposite;
    }


public:
    SnakeGame() {
        init();
        set_direction(Game::right);
        std::cout << ANSI_CODES::hide_cursor << std::flush; // hide caret in terminal
    }

    ~SnakeGame() {
        std::cout << ANSI_CODES::show_cursor << std::flush; // show caret in terminal
        if (thread.joinable()) {
            thread.join(); // wait to complete thread task
        }
    }

    /**
     * If you change directions too fast, for example, moving up, then quickly pressing left and
     * down - only the last input ('down') gets applied. To avoid this problem and game end (because snake start move opposite)
     * we have direction_candidate variable. We set direction to direction_candidate and when next tick comes (before next move)
     * we set new direction value from direction_candidate (after validation of course)
     */
    void set_direction(const Game::Direction dir) {
        std::scoped_lock l(state_mutex);
        direction_candidate = dir;

        run_loop(Game::GAME_SPEED_MS);
    }

    Game::Direction get_direction() const {return direction;}

    void print_changes() {
        std::scoped_lock l(state_mutex);
        ANSI_CODES::set_caret_position(head.y + 1, head.x + 1); // +1, because board starts from 0, but terminal row starts from 1
        std::cout << Game::symbols.snake_color << Game::symbols.snake << std::flush << ANSI_CODES::reset;


        auto const current_tail = snake_body.back();
        if (current_tail.x != old_tail.x || current_tail.y != old_tail.y) {
            ANSI_CODES::set_caret_position(old_tail.y + 1, old_tail.x + 1); // +1, because board starts from 0, but terminal row starts from 1
            std::cout << Game::symbols.field << std::flush;
        }

        ANSI_CODES::set_caret_position(food.y + 1, food.x + 1); // +1, because board starts from 0, but terminal row starts from 1
        std::cout << Game::symbols.food_color << Game::symbols.food << std::flush  << ANSI_CODES::reset;

    }

    void print_score() const {
        ANSI_CODES::set_caret_position(score_position.y, score_position.x);
        std::cout << "Score: " << state.score << std::flush;
    }

    void print_initial_board() {
        std::scoped_lock l(state_mutex);

        for (auto& row : board) {
            for (const auto col : row) {
                if (col == Game::symbols.boundary) {
                    std::cout << Game::symbols.boundary_color  << col << ANSI_CODES::reset;
                } else  if (col == Game::symbols.snake){
                    std::cout << Game::symbols.snake_color  << col << ANSI_CODES::reset;
                } else {
                    std::cout << col;
                }
            }
            std::cout << '\n';
        }
    }

};

void setup_terminal_for_windows() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);


    // Block enter characters in terminal. If you try to enter something - character will not be printed, but we can get thai value by _getch()
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);

    // We are disabling ENABLE_ECHO_INPUT to prevent characters from appearing on the screen and ENABLE_LINE_INPUT
    // so that we don't have to wait for the user to press Enter.
    SetConsoleMode(hStdin, mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));
#endif
}


/**
 * The core idea of this solution is to update only the specific parts of the board that have changed.
 * We can achieve this using ANSI escape codes, which allow us to position the cursor at specific coordinates:
 * std::cout << "\033[" << y << ";" << x << "H";
 * This command sets the absolute position of the cursor y rows from the top and x columns from the
 * left enabling us to overwrite only the necessary characters.
 */

int main() {
    setup_terminal_for_windows();

    // TODO Make this program crossplatform, move windows only part outside, and check how in works on Linux and Mac

    SnakeGame snake_game {};
    snake_game.print_initial_board();

    while (true) {
        if (_kbhit()) { // Перевіряємо, чи натиснута клавіша (не блокує цикл)
            const auto ch = _getch(); // Отримуємо символ без відображення на екрані
            if (ch == 27) break;

            Game::Direction direction = {};
            if (ch == 'w') direction = Game::Direction::up;
            else if (ch == 'a') direction = Game::Direction::left;
            else if (ch == 's') direction = Game::Direction::down;
            else if (ch == 'd') direction = Game::Direction::right;

            snake_game.set_direction(direction);
        }
    }


    return 0;
}
