#ifndef BUTTONS_H
#define BUTTONS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct Button
{
  SDL_Texture *texture;
  SDL_Rect rect;
};

#define SELECTION_TOTAL 7
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
  Button selection[SELECTION_TOTAL];
};

void makeButton(SDL_Renderer *renderer, Button *button,
                const char *text, TTF_Font *font, SDL_Color text_color);
void setButtonPosition(Button *button, int x, int y);
void DrawButtonOutline(SDL_Renderer *renderer, SDL_Rect outline, int pad, int width);

void CreateButtonsForLabel(SDL_Renderer *renderer, Label *label, int nButtons,
                           char text[][20], TTF_Font *font, SDL_Color text_color);

bool checkPointRectCollision(int x, int y, SDL_Rect rect);

#endif
