#ifndef TITLE_H
#define TITLE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "gamestate.h"
#include "buttons.h"
#include "timer.h"

class Title : public GameState
{
public:
  void init(GameEngine* ge);
  void quit();

  void handleEvents(GameEngine* ge);
  void update();
  void render(GameEngine* ge);

  void selectOption(GameEngine* ge);

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

  Button bTitle;
  union
  {
    Button buttons[4];
    struct
    {
      Button vs_human, vs_bot, options, exit;
    };
  };

  Timer startup;
  bool ready;

  int option;
};

#endif
