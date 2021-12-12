
#include "lira.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {

    auto game = Lira::LiraGame(320, 180, 800, 480, "Lira");
    game.run();
    return 0;
}
