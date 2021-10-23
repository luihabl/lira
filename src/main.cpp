
#include "microninja.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {

    auto game = MicroNinja::MicroNinjaGame(320, 180, 800, 480, "Lira");
    game.run();
    return 0;
}
