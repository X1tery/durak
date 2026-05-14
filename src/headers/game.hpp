#pragma once
#include <player.hpp>
#include <vector>
#include <unordered_map>
#include <deque>

int toVal(std::string sym);
char toSym(int val);

enum suits {
    DIAMONDS,
    CLUBS,
    HEARTS,
    SPADES
};

std::unordered_map<suits, char32_t> symbols {
    {DIAMONDS, '♦'},
    {CLUBS, '♣'},
    {HEARTS, '♥'},
    {SPADES, '♠'}
};

struct Card {
    suits suit;
    int value;
    // set 5th bit of value for trump
    Card(std::string _value, suits suit);
};

struct Game {
    std::vector<Player> plrs;
    std::deque<Card> deck;
    suits trump;
    int turnno;
    Player plrturn;
    Game(std::vector<Player> _plrs);
};