#include <server.hpp>
#include <utils.hpp>
#include <const.hpp>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <chrono>
#include <print>

int initServer(const char* port) {
    int sockfd;
    addrinfo hints, *info, *inode;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(NULL, port, &hints, &info) != 0)
        except("FAILED TO GETADDRINFO");
    for (inode = info; inode != nullptr; inode = inode->ai_next) {
        if ((sockfd = socket(info->ai_family, info->ai_socktype, info->ai_protocol)) == -1)
            continue;
        if (bind(sockfd, info->ai_addr, info->ai_addrlen) == -1)
            continue;
        break;
    }
    freeaddrinfo(info);
    if (inode == nullptr)
        except("FAILED TO BIND SOCKET");
    SERV_PORT = port;
    return sockfd;
}

std::vector<Player*> getPlayers(const int sockfd) {
    std::vector<Player*> plrs{};
    size_t iplr;
    auto sendStatus{[&]() -> void {
        for (Player* plr : plrs) {
            plr->sendmsg("\033[2J\033[H-------------------------\n");
            plr->sendmsg("          DURAK\n");
            plr->sendmsg("-------------------------\n");
            plr->sendmsg("PLAYERS:\n");
            for (Player* plri : plrs)
                plr->sendmsg(std::format(" - {}\n", plri->getName()));
            plr->sendmsg("-------------------------\n");
            plr->sendmsg(std::format("WAITING FOR PLAYERS ({}/{})\n", iplr, PLR_COUNT));
            plr->sendmsg("-------------------------\n");
        }
    }};
    std::future<void> listen_thread{std::async(std::launch::async, [&]() -> void {
        while (true) {
            std::this_thread::sleep_for(UPD_CLOCK);
            for (size_t i = 0; i < plrs.size(); i++) {
                if (!plrs[i]->isHere()) {
                    delete plrs[i];
                    iplr--;
                    plrs.erase(plrs.cbegin() + i);
                    printStatus(SERV_PORT, plrs, std::format("WAITING FOR PLAYERS ({}/{})", iplr, PLR_COUNT));
                    sendStatus();
                }
            }
            if (plrs.size() == PLR_COUNT) return;
        }
        return;
    })};
    for (iplr = 0; iplr < PLR_COUNT; iplr = plrs.size()) {
        printStatus(SERV_PORT, plrs, std::format("WAITING FOR PLAYERS ({}/{})", iplr, PLR_COUNT));
        sendStatus();
        if (listen(sockfd, 3) == -1)
            except("FAILED TO BIND SOCKET!");
        int plrsockfd;
        sockaddr addr;
        socklen_t addrlen;
        if ((plrsockfd = accept(sockfd, &addr, &addrlen)) == -1)
            continue;
        bool done{false};
        std::string name{};
        char buff[BUFF_SIZE];
        while (!done) {
            if (recv(plrsockfd, &buff, BUFF_SIZE, 0) <= 0) break;
            for (char c : buff) {
                if (c == '\n') {
                    Player* newplr = new Player(name, plrsockfd, addr);
                    plrs.push_back(newplr);
                    done = true;
                    break;
                } else 
                    name.push_back(c);
            }
        }
    }
    listen_thread.get();
    printStatus(SERV_PORT, plrs, "GAME IN PROGRESS");
    for (Player* plr : plrs)
        plr->sendmsg("\033[2J\033[H\n");
    return plrs;
}