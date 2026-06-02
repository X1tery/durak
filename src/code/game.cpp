#include <game.hpp>
#include <player.hpp>
#include <card.hpp>
#include <utils.hpp>
#include <const.hpp>
#include <cwctype>
#include <random>
#include <utility>
#include <format>
#include <print>

void Game::draw() {
    if (deck.size() > 0) {
        for (Player* plr : plrs) {
            if (plr->hand.size() < 6) {
                std::string msg{"DRAWN"};
                while (plr->hand.size() < 6 && deck.size() > 0) {
                    plr->hand.push_back(deck.front());
                    msg.push_back(' ');
                    msg.append(deck.front().str());
                    deck.pop_front();
                }
                msg.push_back('\n');
                plr->sendmsg(msg);
            }
        }
        if (deck.size() == 0) {
            shout("THE DECK HAS BEEN DEPLETED!");
            return;
        }
    }
}

void Game::place(Player* who, Card card) {
    if (plrs[plratt] == who) {
        if (table.size() == 0 || std::count_if(table.begin(), table.end(), [&](Card tcard) {return (0xf & tcard.getValue()) == (0xf & card.getValue());})) {
            shout(std::format("{} PLACED {}", who->getName(), card.str()));
            table.push_front(card);
            who->hand.erase(std::find(who->hand.begin(), who->hand.end(), card));
            plrs[plrdef]->sendmsg("YOUR TURN!\n");
            plrturn = plrdef;
        } else {
            who->sendmsg(std::format("YOU CANT PLACE {}!\n", card.str()));
        }
    } else if (plrs[plrdef] == who) {
        if (card > table.front()) {
            shout(std::format("{} PLACED {}", who->getName(), card.str()));
            table.push_front(card);
            who->hand.erase(std::find(who->hand.begin(), who->hand.end(), card));
            plrs[plratt]->sendmsg("YOUR TURN!\n");
            plrturn = plratt;
        } else {
            who->sendmsg(std::format("YOU CANT PLACE {}!\n", card.str()));
        }
    }
}

void Game::endTurn(Player* loser) {
    roundno++;
    if (loser == nullptr) {
        shout("BEAT!");
        table.clear();
        plratt = (plratt + 1) % PLR_COUNT;
        plrdef = (plratt + 1) % PLR_COUNT;
        plrturn = plratt;
    } else {
        shout(std::format("{} LOST THE ATTACK!", loser->getName()));
        std::string msg{"DRAWN"};
        while (table.size() > 0) {
            loser->hand.push_back(table.front());
            msg.push_back(' ');
            msg.append(table.front().str());
            table.pop_front();
        }
        msg.push_back('\n');
        loser->sendmsg(msg);
        plratt = (plrdef + 1) % PLR_COUNT;
        plrdef = (plratt + 1) % PLR_COUNT;
        plrturn = plratt;
    }
    if (plrs[plratt]->hand.size() == 0 && deck.size() == 0) {
        PLR_COUNT--;
        plrs[plratt]->sendmsg("win\n");
        shout(std::format("{} HAS BEEN SUCCESSFULLY ELIMINATED!", plrs[plratt]->getName()));
        plrs.erase(plrs.begin() + plratt);
    } else if (plrs[plrdef]->hand.size() == 0 && deck.size() == 0) {
        PLR_COUNT--;
        plrs[plrdef]->sendmsg("win\n");
        shout(std::format("{} HAS BEEN SUCCESSFULLY ELIMINATED!", plrs[plrdef]->getName()));
        plrs.erase(plrs.begin() + plrdef);
    }
    if (PLR_COUNT == 1) {
        plrs[0]->sendmsg("lose\n");
        end();
        return;
    } else if (PLR_COUNT == 0) {
        end();
        return;
    }
    draw();
    shout(std::format("{} ATTACKS {}", plrs[plratt]->getName(), plrs[plrdef]->getName()));
    plrs[plrturn]->sendmsg("YOUR TURN!\n");
}

void Game::shout(std::string msg) {
    msg.push_back('\n');
    for (Player* plr : plrs)
        plr->sendmsg(msg);
}

void Game::action(Player* who, std::string msg) {
    if (!msg.empty()) {
        if (msg[0] == '/') {
            if (msg == "/help") {
                who->sendmsg(HELP_MSG);
            } else if (msg == "/clear") {
                who->sendmsg("\033[2J\033[H\n");
            } else if (msg == "/hand") {
                std::string handmsg{"HAND"};
                for (Card card : who->hand)
                    handmsg.append(std::format(" {}", card.str()));
                handmsg.push_back('\n');
                who->sendmsg(handmsg);
            } else if (msg == "/table") {
                std::string table_cards{"TABLE"};
                for (Card card : table)
                    table_cards.append(std::format(" {}", card.str()));
                table_cards.push_back('\n');
                if (table_cards != "TABLE")
                    who->sendmsg(table_cards);
                else
                    who->sendmsg("THE TABLE IS EMPTY!\n");
            } else if (msg == "/trump") {
                who->sendmsg(std::format("TRUMP {}\n", trump_card.str()));
            } else if (msg == "/left") {
                for (Player* plr : plrs) {
                    if (plr != who)
                        who->sendmsg(std::format("{} HAS {}\n", plr->getName(), plr->hand.size()));
                }
                if (deck.size() > 0 )
                    who->sendmsg(std::format("LEFT IN THE DECK {}\n", deck.size()));
                else
                    who->sendmsg("THE DECK HAS BEEN DEPLETED!\n");
            } else if (msg == "/info") {
                std::string your_cards{"HAND"};
                for (Card card : who->hand)
                    your_cards.append(std::format(" {}", card.str()));
                your_cards.push_back('\n');
                who->sendmsg(your_cards);
                std::string table_cards{"TABLE"};
                for (Card card : table)
                    table_cards.append(std::format(" {}", card.str()));
                table_cards.push_back('\n');
                if (table_cards != "TABLE")
                    who->sendmsg(table_cards);
                else
                    who->sendmsg("THE TABLE IS EMPTY!\n");
                who->sendmsg(std::format("TRUMP {}\n", trump_card.str()));
                for (Player* plr : plrs) {
                    if (plr != who)
                        who->sendmsg(std::format("{} HAS {}\n", plr->getName(), plr->hand.size()));
                }
                if (deck.size() > 0 )
                    who->sendmsg(std::format("LEFT IN THE DECK {}\n", deck.size()));
                else
                    who->sendmsg("THE DECK HAS BEEN DEPLETED!\n");
            } else if (msg.size() > 7 && msg.substr(0, 6) == "/place") {
                if (plrs[plrturn] == who) {
                    Card pcard{msg.substr(7)};
                    if (pcard.getSuit() == trump)
                        pcard.makeTrump();
                    bool has{false};
                    for (Card card : who->hand) {
                        if (card == pcard) {
                            place(who, card);
                            has = true;
                            break;
                        }
                    }
                    if (!has) who->sendmsg(std::format("YOU DON'T POSSESS {}!\n", msg.substr(7)));
                } else {
                    who->sendmsg("IT'S NOT YOUR TURN!\n");
                }
            } else if (msg == "/beat") {
                if (plrs[plratt] == who) {
                    if (plrs[plrturn] == who)
                        endTurn();
                    else
                        who->sendmsg("IT'S NOT YOUR TURN!\n");
                } else
                    who->sendmsg("YOU ARE NOT THE ATTACKER!\n");
            } else if (msg == "/take") {
                if (plrs[plrdef] == who) {
                    if (plrs[plrturn] == who)
                        endTurn(who);
                    else
                        who->sendmsg("IT'S NOT YOUR TURN!\n");
                } else
                    who->sendmsg("YOU ARE NOT THE DEFENDER!\n");
            } else {
                who->sendmsg(std::format("UNKNOWN COMMAND \"{}\"\n", msg));
            }
        } else
            shout(std::format("{}: {}", who->getName(), msg));
    }
}

void Game::disconnect(Player* who) {
    if (std::count(plrs.begin(), plrs.end(), who)) {
        shout(std::format("{} HAS DISCONNECTED!", who->getName()));
        PLR_COUNT--;
        if (plrs[plratt] == who) {
            std::string msg{"DRAWN"};
            bool isdef{plrs[plrturn] == who};
            while (table.size() > 0) {
                if (isdef) {
                    plrs[plrdef]->hand.push_back(table.front());
                    msg.append(std::format(" {}", table.front().str()));
                }
                table.pop_front();
                isdef = !isdef;
            }
            msg.push_back('\n');
            if (msg != "DRAWN\n")
                plrs[plrdef]->sendmsg(msg);
            plrs.erase(std::find(plrs.begin(), plrs.end(), who));
            if (PLR_COUNT < 2) {
                plrs[0]->sendmsg("THE GAME HAS BEEN CANCELLED DUE TO LOW PLAYER COUNT\n");
                plrs[0]->sendmsg("win\n");
                end();
                return;
            }
            plratt %= PLR_COUNT;
            plrdef = (plratt + 1) % PLR_COUNT;
            plrturn = plratt;
            draw();
            shout(std::format("{} ATTACKS {}", plrs[plratt]->getName(), plrs[plrdef]->getName()));
            plrs[plrturn]->sendmsg("YOUR TURN!\n");
        } else if (plrs[plrdef] == who) {
            std::string msg{"DRAWN\n"};
            bool isatt{plrs[plrturn] == who};
            while (table.size() > 0) {
                if (isatt) {
                    plrs[plratt]->hand.push_back(table.front());
                    msg.append(std::format(" {}", table.front().str()));
                }
                table.pop_front();
                isatt = !isatt;
            }
            msg.push_back('\n');
            if (msg != "DRAWN")
                plrs[plratt]->sendmsg(msg);
            plrs.erase(std::find(plrs.begin(), plrs.end(), who));
            if (PLR_COUNT < 2) {
                plrs[0]->sendmsg("THE GAME HAS BEEN CANCELLED DUE TO LOW PLAYER COUNT\n");
                plrs[0]->sendmsg("win\n");
                end();
                return;
            }
            plratt = plrdef % PLR_COUNT;
            plrdef = (plratt + 1) % PLR_COUNT;
            plrturn = plratt;
            draw();
            shout(std::format("{} ATTACKS {}", plrs[plratt]->getName(), plrs[plrdef]->getName()));
            plrs[plrturn]->sendmsg("YOUR TURN!\n");
        } else {
            plrs.erase(std::find(plrs.begin(), plrs.end(), who));
            if (PLR_COUNT < 2) {
                plrs[0]->sendmsg("THE GAME HAS BEEN CANCELLED DUE TO LOW PLAYER COUNT\n");
                plrs[0]->sendmsg("win\n");
                end();
                return;
            }
        }
        printStatus(SERV_PORT, plrs, "GAME IN PROGRESS");
    }
}

void Game::end() {
    end_mutex->unlock();
}

Game::Game(std::vector<Player*> _plrs, std::mutex* _end_mutex) : end_mutex(_end_mutex), table({}), roundno(1), plrturn(0), plratt(0), plrdef(1) {
    for (Player* plr : _plrs) {
        plrs.push_back(plr);
        plr->game = reinterpret_cast<void*>(this);
    }
    deck = getShuffled();
    trump_card = deck.back();
    trump = trump_card.getSuit();
    for (Card& card : deck)
        if (card.getSuit() == trump) card.makeTrump();
    draw();
    shout(std::format("{} ATTACKS {}", plrs[plratt]->getName(), plrs[plrdef]->getName()));
    plrs[plrturn]->sendmsg("YOUR TURN!\n");
}