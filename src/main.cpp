
#include "game.h"

int main(int argc, char *argv[]) {

    auto game = MicroNinja::MicroNinjaGame(400, 240, 800, 480, "Micro Ninja");
    game.run();
    return 0;
}
