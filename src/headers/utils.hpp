#pragma once
#include <player.hpp>
#include <string>
#include <vector>

void except(std::string msg, int exit_code = 1);
void printStatus(const char* serv_port, std::vector<Player*> plrs, std::string status);