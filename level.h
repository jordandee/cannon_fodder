#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "gamestate.h"
#include "terrain.h"

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

  SDL_Texture* cannon;
  SDL_Rect cannon_rectL;
  SDL_Rect cannon_rectR;

  SDL_Texture* shaft;
  SDL_Rect shaft_rectL;
  SDL_Rect shaft_rectR;

  double rotation_angleL;
  SDL_Point rotation_pointL;
};

#endif
