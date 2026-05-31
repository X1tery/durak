#include <utils.hpp>
#include <player.hpp>
#include <print>
#include <iostream>

void except(std::string msg, int exit_code) {
    std::println(std::cerr, "{}", msg);
    exit(exit_code);
}

void printStatus(const char* serv_port, std::vector<Player*> plrs, std::string status) {
    std::println(
        "\033[2J\033[H"
        "-------------------------\n"
        "      DURAK SERVER\n"
        "-------------------------\n"
        "PORT: {}\n"
        "-------------------------\n"
        "PLAYERS:",
        serv_port);
    for (Player* plr : plrs)
        std::println(" - {}", plr->getName());
    std::println(
        "-------------------------\n"
        "{}\n"
        "-------------------------",
        status);
}