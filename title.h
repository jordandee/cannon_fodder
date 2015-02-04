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

  TTF_Font *font48, *font24;
  SDL_Surface *text_surf;
  SDL_Color text_color;

  SDL_Texture *title_texture;
  SDL_Rect title_rect;
  SDL_Texture *vs_human_texture, *vs_bot_texture, *options_texture, *exit_texture;
  SDL_Rect vs_human_rect, vs_bot_rect, options_rect, exit_rect;
};

#endif
