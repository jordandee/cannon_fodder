#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "gamestate.h"
#include "terrain.h"
#include "timer.h"
#include "cannon.h"
#include "ball.h"

class Level : public GameState
{
public:
  void init(GameEngine* ge);
  void quit();

  void handleEvents(GameEngine* ge);
  void update();
  void render(GameEngine* ge);

  static Level* Instance() { return &_s; }

protected:
  Level() { }

private:
  static Level _s;

  std::vector<Pixel> terrain;

  Cannon cannonL, cannonR;

  Ball ball;

  Timer timer;
  double dt;
};

#endif
