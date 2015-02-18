#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SDL2/SDL.h>

class GameState;

class GameEngine
{
public:

  void init();
  void quit();

  void changeState(GameState* gamestate);

  void handleEvents();
  void update();
  void render();

  bool running() { return _running; }
  void stop() { _running = false; }

  SDL_Window* win = NULL;
  SDL_Renderer* renderer = NULL;
private:
  GameState* gs = NULL;

  bool _running = true;
};

#endif
