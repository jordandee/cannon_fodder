// A clone of the classic Mac game Cannon Fodder
// Original game by John Barnett, Fantasy Computing, 1989
// Clone by Jordan Dee McConnell, 2014

#include "gameengine.h"
//#include "title.h"
#include "level.h"

int main (int argc, char *argv[])
{
  GameEngine game;

  game.init();

  game.changeState(Level::Instance());

  while (game.running())
  {
    game.handleEvents();
    game.update();
    game.render();
  }

  game.quit();
  return 0;
}
