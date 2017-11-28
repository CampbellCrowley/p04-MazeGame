#include "Common.h"
#include "Game.h"

Game::GameController game_;
int main(int argc, const char* argv[]) {
  game_.Initialize(argc, argv);
  game_.CheckPrevious();
  return game_.Run();
}
