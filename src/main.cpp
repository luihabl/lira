
#include "lira.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {

    auto game = Lira::LiraGame(320, 180, 960, 540, "Lira");
    game.run();
    return 0;
}
