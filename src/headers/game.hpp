#pragma once
#include <player.hpp>
#include <card.hpp>
#include <vector>
#include <string>
#include <unordered_map>
#include <deque>
#include <mutex>

struct Game {
    Game(std::vector<Player*> _plrs, std::mutex* _end_mutex);
    void draw();
    void place(Player* who, Card card);
    void endTurn(Player* loser = nullptr);
    void shout(std::string msg);
    void action(Player* who, std::string msg);
    void disconnect(Player* who);
    void end();
    std::mutex* end_mutex;
    std::vector<Player*> plrs;
    std::deque<Card> deck;
    std::deque<Card> table;
    Card trump_card;
    suits trump;
    int roundno;
    int plrturn;
    int plratt;
    int plrdef;
};