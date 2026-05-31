#pragma once
#include <player.hpp>
#include <game.hpp>
#include <vector>
#include <chrono>

int initServer(const char* port);
std::vector<Player*> getPlayers(const int sockfd);