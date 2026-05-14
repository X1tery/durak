#include <player.hpp>
#include <server.hpp>
#include <sys/socket.h>
#include <unistd.h>

Player::Player(int _sockfd, sockaddr _addr, std::string _name) : sockfd(_sockfd), addr(_addr), name(_name) {}

std::string Player::getName() {
    return name;
}

std::string Player::receive() {
    char buff[BUFF_SIZE];
    recv(sockfd, buff, BUFF_SIZE, 0);
    return static_cast<std::string>(buff);
}

int Player::sendmsg(char* buff) {
    return send(sockfd, buff, BUFF_SIZE, 0);
}