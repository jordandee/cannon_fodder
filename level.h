#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "gamestate.h"
#include "terrain.h"
#include "timer.h"
#include "cannon.h"

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

  SDL_Texture* ball;
  SDL_Rect ball_rect;
  double bx, by, bv, ba;

  Timer timer;
  double dt;
};

#endif
