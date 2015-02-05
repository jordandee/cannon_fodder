#ifndef OPTIONS_H
#define OPTIONS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "gamestate.h"

#define OPTION_TOTAL 7

struct Button
{
  SDL_Texture *texture;
  SDL_Rect rect;
};

struct Label
{
  union
  {
    Button button;
    struct
    {
    SDL_Texture *texture;
    SDL_Rect rect;
    };
  };
  int active_selection;
  int valid_selections;
  Button selection[OPTION_TOTAL];
};

class Options : public GameState
{
public:
  void init(GameEngine* ge);
  void quit();

  void handleEvents(GameEngine* ge);
  void update();
  void render(GameEngine* ge);

  void selectOption(GameEngine* ge);

  static Options* Instance() { return &_s; }

protected:
  Options() { }

private:
  static Options _s;

  SDL_Texture* background_texture;
  SDL_Rect background_rect;

  TTF_Font *font48, *font24;

  Button title;
  union
  {
    Label labels[4];
    struct
    {
      Label terrain_type;
      Label obstacles;
      Label wind;
      Label fullscreen;
    };
  };
  //Button title, terrain_type, obstacles, wind, fullscreen;

  int option;
};

#endif
