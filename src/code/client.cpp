#include <client.hpp>
#include <const.hpp>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <print>

void except(std::string msg, int exit_code) {
    std::println(std::cerr, "{}", msg);
    exit(exit_code);
}

std::string getName() {
    std::string name{};
    while (name.size() == 0) {
        std::println("ENTER YOUR NICKNAME:");
        std::cin >> name;
        if (name.size() > MAX_NAME_SIZE) {
            name.clear();
            std::println("TOO LONG OF A NAME! (MAX {})", MAX_NAME_SIZE);
        }
    }
    name.push_back('\n');
    return name;
}

addrinfo* getAddr() {
    std::string addr, port;
    std::println("ENTER SERVER IP:");
    std::cin >> addr;
    std::println("ENTER SERVER PORT:");
    std::cin >> port;
    addrinfo hints, *info;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = 0;
    if (getaddrinfo(addr.c_str(), port.c_str(), &hints, &info) != 0)
        except("SERVER NOT FOUND!");
    return info;
}

int connectServ(addrinfo* info) {
    int sockfd;
    addrinfo* inode;
    for (inode = info; inode != nullptr; inode = inode->ai_next) {
        if ((sockfd = socket(inode->ai_family, inode->ai_socktype, inode->ai_protocol)) == -1)
            continue;
        if (connect(sockfd, inode->ai_addr, inode->ai_addrlen) == -1)
            continue;
        break;
    }
    freeaddrinfo(info);
    if (inode == nullptr)
        except("FAILED TO CONNECT TO SERVER!");
    return sockfd;
}

void recvLoop(int sockfd) {
    std::string msg{};
    while (true) {
        char buff[BUFF_SIZE];
        int read;
        if ((read = recv(sockfd, buff, BUFF_SIZE, 0)) <= 0) {
            except("CONNECTION TERMINATED!");
        }
        for (int i = 0; i < read; i++) {
            if (buff[i] == '\n') {
                if (msg == "win") {
                    close(sockfd);
                    except("YOU'VE BEEN SUCCESSFULLY ELIMINATED!", 0);
                } else if (msg == "lose") {
                    except("YOU'VE LOST!", 0);
                } else std::println("{}", msg);
                msg.clear();
            } else
                msg.push_back(buff[i]);
        }
    }
}

void sendLoop(int sockfd) {
    while (true) {
        std::string msg{};
        std::getline(std::cin, msg);
        std::print("\033[1A\033[2K\r");
        if (!msg.empty() && msg[0] == '/') {
            if (ACTIONS.contains(msg) || (msg.size() > 7 && msg.substr(0, 6) == "/place")) {
                msg.push_back('\n');
                if (send(sockfd, msg.c_str(), msg.size(), 0) == -1)
                    except("CONNECTION TERMINATED");
            } else
                std::println("UNKNOWN COMMAND \"{}\"", msg);
        } else if (!msg.empty()) {
            msg.push_back('\n');
            if (send(sockfd, msg.c_str(), msg.size(), 0) == -1)
                except("CONNECTION TERMINATED");
        }
        msg.clear();
    }
}