#pragma once
#include <game.hpp>
#include <sys/socket.h>
#include <string>
#include <future>

class Player {
private:
    int sockfd;
    sockaddr addr;
    std::string name;
    std::future<void> recvloop;
public:
    Player(int _sockfd, sockaddr _addr, std::string _name);
    Game* game;
    std::string getName();
    std::string receive();
    int sendmsg(char* data);
};