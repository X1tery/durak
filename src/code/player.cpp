#include <player.hpp>
#include <game.hpp>
#include <server.hpp>
#include <const.hpp>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include <print>
#include <future>

Player::Player(std::string _name, int _sockfd, sockaddr _addr) : name(_name), sockfd(_sockfd), addr(_addr), hand({}), game(nullptr), isAttacking(false) {
    recv_loop = std::async(std::launch::async, [&]() -> void {
        while (true) {
            std::string msg{recvmsg()};
            if (msg == "\n") {
                remove();
                return;
            } else
                reinterpret_cast<Game*>(game)->action(this, msg);
        }
    });
}

std::string Player::getName() const {
    return name;
}

bool Player::isHere() const {
    return (sockfd == -1) ? false : true;
}

void Player::remove() {
    close(sockfd);
    sockfd = -1;
    if (game != nullptr)
        reinterpret_cast<Game*>(game)->disconnect(this);
}

std::string Player::recvmsg() {
    std::string msg{};
    while (true) {
        char buff[BUFF_SIZE];
        if (recv(sockfd, buff, BUFF_SIZE, 0) <= 0)
            return "\n";
        if (game != nullptr) {
            for (char c : buff) {
                if (c == '\n') {
                    return msg;
                } else 
                    msg.push_back(c);
            }
        }
    }
}

int Player::sendmsg(std::string msg) {
    return send(sockfd, msg.c_str(), msg.size(), 0);
}