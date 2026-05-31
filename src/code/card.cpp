#include <card.hpp>
#include <vector>
#include <deque>
#include <random>
#include <format>

const std::deque<Card> STDECK = {
    "6D", "7D", "8D", "9D", "10D", "JD", "QD", "KD", "AD",
    "6C", "7C", "8C", "9C", "10C", "JC", "QC", "KC", "AC",
    "6H", "7H", "8H", "9H", "10H", "JH", "QH", "KH", "AH",
    "6S", "7S", "8S", "9S", "10S", "JS", "QS", "KS", "AS"
};

int toVal(std::string sym) {
    if (sym.size() != 1) {
        if (sym == "10") return 10;
        else return 0;
    }
    if (std::isdigit(sym[0]) && sym[0] >= '6' && sym[0] <= '9') return sym[0] - '0';
    switch (sym[0]) {
        case 'j': case 'J':
            return 11;
        case 'q': case 'Q':
            return 12;
        case 'k': case 'K':
            return 13;
        case 'a': case 'A':
            return 14; 
        default:
            return 0;
    }
}

std::string toSym(int val) {
    val %= 0x10;
    if (val >= 6 && val < 10) return {static_cast<char>(val + '0')};
    else if (val == 10) return "10";
    else if (val >= 11 && val <= 14) {
        switch (val) {
            case 11:
                return "J";
            case 12:
                return "Q";
            case 13:
                return "K";
            case 14:
                return "A";
        }
    }
    return "";
}

suits toSuitVal(char sym) {
    switch (sym) {
        case 'D': case 'd':
            return DIAMONDS;
        case 'C': case 'c':
            return CLUBS;
        case 'H': case 'h':
            return HEARTS;
        case 'S': case 's':
            return SPADES;
    }
    return UNKNOWN;
}

std::string toSuitSym(suits suit) {
    switch (suit) {
        case DIAMONDS:
            return "\033[38;5;124m♦\033[0m";
        case CLUBS:
            return "\033[38;5;234m♣\033[0m";
        case HEARTS:
            return "\033[38;5;124m♥\033[0m";
        case SPADES:
            return "\033[38;5;234m♠\033[0m";
        case UNKNOWN:
            return "?";
    }
    return "";
}

Card::Card(std::string c) {
    switch (c.size()) {
        case 2:
            value = toVal(c.substr(0, 1));
            suit = toSuitVal(c[1]);
            break;
        case 3:
            value = toVal(c.substr(0, 2));
            suit = toSuitVal(c[2]);
            break;
        default:
            value = 0;
            suit = UNKNOWN;
            break;
    }
}

Card::Card(const char* cc) {
    std::string c{cc};
    switch (c.size()) {
        case 2:
            value = toVal(c.substr(0, 1));
            suit = toSuitVal(c[1]);
            break;
        case 3:
            value = toVal(c.substr(0, 2));
            suit = toSuitVal(c[2]);
            break;
        default:
            value = 0;
            suit = UNKNOWN;
            break;
    }
}

bool Card::operator==(Card c) const {
    return (value == c.getValue() && suit == c.getSuit());
}

bool Card::operator>(Card c) const {
    if (isTrump() || getSuit() == c.getSuit())
        return value > c.getValue();
    else
        return false;
}

bool Card::operator<(Card c) const {
    if (isTrump() || getSuit() == c.getSuit())
        return value < c.getValue();
    else
        return false;
}

suits Card::getSuit() const {
    return suit;
}

int Card::getValue() const {
    return value;
}

std::string Card::str() const {
    return std::format("\033[48;5;255m\033[38;5;234m{}{}", toSym(getValue()), toSuitSym(getSuit()));
}

bool Card::isTrump() const {
    return (value & 0x10) == 0x10;
}

void Card::makeTrump() {
    value |= 0x10;
}

std::deque<Card> getShuffled() {
    std::deque<Card> deck{STDECK};
    std::random_device dev{};
    std::mt19937 gen{dev()};
    for (size_t i = 0; i < deck.size(); i++) {
        std::uniform_int_distribution<size_t> dist{0, i};
        std::swap(deck[i], deck[dist(gen)]);
    }
    return deck;
}