#include <server.hpp>
#include <error.hpp>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <print>

std::string serv_port{};
const size_t BUFF_SIZE{127};

void printStatus(std::vector<Player> plrs, std::string status) {
    std::println(
        "\033[2J\033[H"
        "-------------------------\n"
        "PORT: {}\n"
        "-------------------------\n"
        "PLAYERS:",
        serv_port);
    for (Player& plr : plrs)
        std::println("{}", plr.getName());
    std::println(
        "-------------------------\n"
        "{}\n"
        "-------------------------",
        status);
}

int initServer(char* port) {
    int sockfd;
    addrinfo hints, *info, *inode;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(NULL, port, &hints, &info) != 0)
        except("failed to getaddrinfo");
    for (inode = info; inode != nullptr; inode = inode->ai_next) {
        if ((sockfd = socket(info->ai_family, info->ai_socktype, info->ai_protocol)) == -1)
            continue;
        if (bind(sockfd, info->ai_addr, info->ai_addrlen) == -1)
            continue;
        break;
    }
    freeaddrinfo(info);
    if (inode == nullptr)
        except("failed to bind socket");
    serv_port = static_cast<std::string>(port);
    return sockfd;
}

std::vector<Player> getPlayers(int sockfd, size_t plrcnt) {
    std::vector<Player> plrs{};
    for (size_t iplr = 0; iplr < plrcnt; iplr = plrs.size()) {
        printStatus(plrs, std::format("WAITING FOR PLAYERS ({}/{})", iplr, plrcnt));
        int plrsockfd;
        sockaddr addr;
        socklen_t addrlen;
        if ((plrsockfd = accept(sockfd, &addr, &addrlen)) == -1)
            continue;
    }
}

void initGame() {
    
}