
#include "microninja.h"

int main(int argc, char *argv[]) {

    auto game = MicroNinja::MicroNinjaGame(320, 180, 800, 480, "Lira");
    game.run();
    return 0;
}
