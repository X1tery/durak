#include <server.hpp>
#include <print>
#include <thread>
#include <chrono>
#include <unistd.h>

int main(int argc, char** argv) {
    initServer(argv[1]);
    printStatus({{3, "X1tery"}, {3, "Enemy"}}, "WAITING FOR PLAYERS");
    return 0;
}