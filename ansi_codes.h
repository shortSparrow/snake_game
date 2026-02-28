#ifndef SNAKE_GAME_ANSI_CODES_H
#define SNAKE_GAME_ANSI_CODES_H
#include <string_view>

namespace ANSI_CODES {
    // reset all settings and styles
    constexpr std::string_view reset {"\033[0m"};
    constexpr std::string_view hide_cursor {"\033[?25l"};
    constexpr std::string_view show_cursor {"\033[?25h"};

    inline void set_caret_position(int y,int x) {
        std::cout << "\033[" << y << ";" << x << "H";
    }


    namespace Color {
        // Standard colors
        constexpr std::string_view black      {"\033[30m"};
        constexpr std::string_view red        {"\033[31m"};
        constexpr std::string_view green      {"\033[32m"};
        constexpr std::string_view yellow     {"\033[33m"};
        constexpr std::string_view blue       {"\033[34m"};
        constexpr std::string_view magenta    {"\033[35m"};
        constexpr std::string_view cyan       {"\033[36m"};
        constexpr std::string_view white      {"\033[37m"};

        // Bold colors
        constexpr std::string_view bold_black   {"\033[1;30m"};
        constexpr std::string_view bold_red     {"\033[1;31m"};
        constexpr std::string_view bold_green   {"\033[1;32m"};
        constexpr std::string_view bold_yellow  {"\033[1;33m"};
        constexpr std::string_view bold_blue    {"\033[1;34m"};
        constexpr std::string_view bold_magenta {"\033[1;35m"};
        constexpr std::string_view bold_cyan    {"\033[1;36m"};
        constexpr std::string_view bold_white   {"\033[1;37m"};

        // Bright colors
        constexpr std::string_view bright_black   {"\033[90m"};
        constexpr std::string_view bright_red     {"\033[91m"};
        constexpr std::string_view bright_green   {"\033[92m"};
        constexpr std::string_view bright_yellow  {"\033[93m"};
        constexpr std::string_view bright_blue    {"\033[94m"};
        constexpr std::string_view bright_magenta {"\033[95m"};
        constexpr std::string_view bright_cyan    {"\033[96m"};
        constexpr std::string_view bright_white   {"\033[97m"};

        // Background colors
        constexpr std::string_view bg_black   {"\033[40m"};
        constexpr std::string_view bg_red     {"\033[41m"};
        constexpr std::string_view bg_green   {"\033[42m"};
        constexpr std::string_view bg_yellow  {"\033[43m"};
        constexpr std::string_view bg_blue    {"\033[44m"};
        constexpr std::string_view bg_magenta {"\033[45m"};
        constexpr std::string_view bg_cyan    {"\033[46m"};
        constexpr std::string_view bg_white   {"\033[47m"};

        // Extra styles
        constexpr std::string_view underline {"\033[4m"};
    }
}



#endif //SNAKE_GAME_ANSI_CODES_H