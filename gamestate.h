#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "gameengine.h"

class GameState
{
public:
  virtual void init(GameEngine* ge) = 0;
  virtual void quit() = 0;

  virtual void handleEvents(GameEngine* ge) = 0;
  virtual void update() = 0;
  virtual void render(GameEngine* ge) = 0;

protected:
  GameState() { }
};

#endif
