#ifndef LEVEL_H
#define LEVEL_H

#include "gamestate.h"

class Level : public GameState
{
public:
  void init();
  void quit();

  void handleEvents(GameEngine* ge);
  void update();
  void render(GameEngine* ge);

  static Level* Instance() { return &_s; }

protected:
  Level() { }

private:
  static Level _s;
};

#endif
