#ifndef TITLE_H
#define TITLE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "gamestate.h"

class Title : public GameState
{
public:
  void init(GameEngine* ge);
  void quit();

  void handleEvents(GameEngine* ge);
  void update();
  void render(GameEngine* ge);

  static Title* Instance() { return &_s; }

protected:
  Title() { }

private:
  static Title _s;

  SDL_Texture* background_texture;
  SDL_Rect background_rect;

  SDL_Texture *text_texture;
  TTF_Font *font;
  SDL_Surface *text_surf;
  SDL_Rect text_rect;
  SDL_Color text_color;
};

#endif
