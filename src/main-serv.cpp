#include <server.hpp>
#include <game.hpp>
#include <card.hpp>
#include <utils.hpp>
#include <const.hpp>
#include <unistd.h>

int main(int argc, char** argv) {
    if (argc != 3)
        except(std::format("{} [PORT] [PLR_COUNT]", argv[0]));
    while (true) {
        int sockfd{initServer(argv[1])};
        PLR_COUNT = std::stoul(static_cast<std::string>(argv[2]));
        std::mutex end_mutex;
        end_mutex.lock();
        std::vector<Player*> plrs{getPlayers(sockfd)};
        Game* game = new Game(plrs, &end_mutex);
        std::lock_guard<std::mutex> end{end_mutex};
        for (Player* plr : plrs) {
            plr->game = nullptr;
            plr->remove();
            plr->recv_loop.get();
            delete plr;
        }
        close(sockfd);
        delete game;
    }
}