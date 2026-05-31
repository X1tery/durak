#include <client.hpp>
#include <const.hpp>
#include <print>
#include <future>

int main() {
    std::string name{getName()};
    addrinfo* info{getAddr()};
    int sockfd{connectServ(info)};
    send(sockfd, name.c_str(), name.size(), 0);
    std::print("\033[2J\033[H");
    std::future<void> send_thread{std::async(std::launch::async, sendLoop, sockfd)};
    std::future<void> recv_thread{std::async(std::launch::async, recvLoop, sockfd)};
    recv_thread.get();
    return 0;
}