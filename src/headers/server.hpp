#pragma once
#include <player.hpp>
#include <vector>

extern std::string serv_port;
extern const size_t BUFF_SIZE;

void printStatus(std::vector<Player> plrs, std::string status);
int initServer(char* port);
std::vector<Player> getPlayers(int sockfd, size_t plrcnt);
void initGame();