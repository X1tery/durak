#include <game.hpp>
#include <cwctype>

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

char toSym(int val) {
    if (val >= 6 && val <= 10) return val + '0';
    else if (val >= 11 && val <= 14) {
        switch (val) {
            case 11:
                return 'J';
            case 12:
                return 'Q';
            case 13:
                return 'K';
            case 14:
                return 'A';
        }
    }
    return 0;
}