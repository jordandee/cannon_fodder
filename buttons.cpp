#include <SDL2/SDL.h>
#include "buttons.h"

void makeButton(SDL_Renderer *renderer, Button *button,
                const char *text, TTF_Font *font, SDL_Color text_color)
{
  TTF_SizeText(font, text, &button->rect.w, &button->rect.h);

  SDL_Surface *text_surf = TTF_RenderText_Solid(font, text, text_color);
  button->texture = SDL_CreateTextureFromSurface(renderer, text_surf);
  SDL_FreeSurface(text_surf);
}

void setButtonPosition(Button *button, int x, int y)
{
  button->rect.x = x;
  button->rect.y = y;
}

void CreateButtonsForLabel(SDL_Renderer *renderer, Label *label, int nButtons,
                           char text[][20], TTF_Font *font, SDL_Color text_color)
{
  int pad = 40;
  label->valid_selections = nButtons;

  int x = 225; // fullscreen label width + pad
  int y = label->rect.y;

  for (int i = 0; i < nButtons; ++i)
  {
    makeButton(renderer, &label->selection[i], text[i], font, text_color);
    setButtonPosition(&label->selection[i], x, y);
    x += label->selection[i].rect.w + pad;
  }
}

void DrawButtonOutline(SDL_Renderer *renderer, SDL_Rect outline, int pad, int width)
{
  outline.x -= pad;
  outline.y -= pad;
  outline.w += 2*pad;
  outline.h += 2*pad;

  for (int i= 0; i < width; ++i)
  {
    SDL_RenderDrawRect(renderer, &outline);
    outline.x += 1;
    outline.y += 1;
    outline.w -= 2;
    outline.h -= 2;
  }
}

bool checkPointRectCollision(int x, int y, SDL_Rect rect)
{
  bool collision_occurred = false;

  if (x >= rect.x && x < (rect.x + rect.w) &&
      y >= rect.y && y < (rect.y + rect.h))
  {
    collision_occurred = true;
  }
  return collision_occurred;
}
