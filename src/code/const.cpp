#include <const.hpp>

size_t PLR_COUNT{};
const size_t MAX_NAME_SIZE{16};
const char* SERV_PORT{};
const size_t BUFF_SIZE{127};
const std::chrono::milliseconds UPD_CLOCK{500};
const std::unordered_set<std::string> ACTIONS{
    "/help",
    "/clear",
    "/hand",
    "/table",
    "/trump",
    "/left",
    "/info",
    "/place",
    "/beat",
    "/take"
};
const std::string HELP_MSG{
    "/help - Shows this help message\n"
    "/clear - Clears the screen\n"
    "/hand - Shows your current hand\n"
    "/table - Shows the cards on the table\n"
    "/trump - Shows the trump card\n"
    "/left - Shows how many cards are left\n"
    "/info - Shows general info about the current game\n"
    "/place - Places a card on the table\n"
    "/beat - Gives up your attack\n"
    "/take - Gives up your defense\n"
};