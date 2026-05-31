#pragma once
#include <card.hpp>
#include <sys/socket.h>
#include <string>
#include <vector>
#include <future>

class Player {
private:
    std::string name;
    int sockfd;
    sockaddr addr;
public:
    Player(std::string _name, int _sockfd = 0, sockaddr _addr = sockaddr(0));
    Player() = default;
    std::future<void> recv_loop;
    void* game;
    bool isAttacking;
    std::vector<Card> hand;
    std::string getName() const;
    bool isHere() const;
    void remove();
    std::string recvmsg();
    int sendmsg(std::string msg);
};