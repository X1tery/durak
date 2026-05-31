#pragma once
#include <string>
#include <deque>

int toVal(std::string sym);
std::string toSym(int val);

enum suits {
    DIAMONDS,
    CLUBS,
    HEARTS,
    SPADES,
    UNKNOWN
};

suits toSuitVal(char sym);
std::string toSuitSym(suits suit);

class Card {
private:
    suits suit;
    int value; // set 5th bit of value for trump
public:
    Card(std::string c);
    Card(const char* cc);
    Card() = default;
    bool operator==(Card c) const;
    bool operator>(Card c) const;
    bool operator<(Card c) const;
    suits getSuit() const;
    int getValue() const;
    std::string str() const;
    bool isTrump() const;
    void makeTrump();
};

extern const std::deque<Card> STDECK;

std::deque<Card> getShuffled();