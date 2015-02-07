#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "gameengine.h"
#include "options.h"
#include "level.h"
#include "resources.h"
#include "title.h"

Options Options::_s;

void makeButton(SDL_Renderer *renderer, Button *button, const char *text, TTF_Font *font)
{
  SDL_Color text_color = {255,255,255};

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

void CreateButtonsForLabel(SDL_Renderer *renderer, Label *label, int nButtons, char text[][20], TTF_Font *font)
{
  int pad = 40;
  label->valid_selections = nButtons;

  int x = 225; // fullscreen label width + pad
  int y = label->rect.y;

  for (int i = 0; i < nButtons; ++i)
  {
    makeButton(renderer, &label->selection[i], text[i], font);
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

  if (x >= rect.x && x < (rect.x + rect.w))
  {
    if (y >= rect.y && y < (rect.y + rect.h))
    {
      collision_occurred = true;
    }
  }
  return collision_occurred;
}

void Options::init(GameEngine* ge)
{
  background_rect = {0, 0, 800, 600};
  background_texture = NULL;
  background_texture = loadTexture("images/screenshot0.png", ge->renderer);

  font48 = NULL;
  font24 = NULL;

  font48 = TTF_OpenFont("fonts/Chicago.ttf", 48);
  font24 = TTF_OpenFont("fonts/Chicago.ttf", 24);

  mouse_moved = false;
  mouseX = 0;
  mouseY = 0;

  title = {};
  terrain_type = {};
  obstacles ={};
  wind = {};
  fullscreen = {};
  highlighted_label = 0;
  highlighted_selection = 0;
  highlight_enable = false;
  labels[0].active_selection = 2;
  labels[1].active_selection = 2;
  labels[2].active_selection = 0;
  labels[3].active_selection = 1;

  makeButton(ge->renderer, &title, "Options", font48);
  setButtonPosition(&title, 100, 100);

  makeButton(ge->renderer, &terrain_type.button, "Terrain", font24);
  setButtonPosition(&terrain_type.button, 50, 250);
  char terrain_settings[][20] = {"Flat","Smooth","Rough","Extreme","???"};
  CreateButtonsForLabel(ge->renderer, &terrain_type, 5, terrain_settings, font24);

  makeButton(ge->renderer, &obstacles.button, "Obstacles", font24);
  setButtonPosition(&obstacles.button, 50, 300);
  char obstacles_settings[][20] = {"None", "Few", "Many", "Lots", "???"};
  CreateButtonsForLabel(ge->renderer, &obstacles, 5, obstacles_settings, font24);

  makeButton(ge->renderer, &wind.button, "Wind", font24);
  setButtonPosition(&wind.button, 50, 350);
  char wind_settings[][20] = {"None", "Slow", "Fast","???"};
  CreateButtonsForLabel(ge->renderer, &wind, 4, wind_settings, font24);

  makeButton(ge->renderer, &fullscreen.button, "FullScreen", font24);
  setButtonPosition(&fullscreen.button, 50, 400);
  char fullscreen_settings[][20] = {"Enabled", "Disabled"};
  CreateButtonsForLabel(ge->renderer, &fullscreen, 2, fullscreen_settings, font24);
}

void Options::quit()
{
}

void Options::handleEvents(GameEngine* ge)
{
  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    if (e.type == SDL_QUIT)
      ge->stop();
    else if (e.type == SDL_KEYDOWN)
    {
      SDL_Keycode key = e.key.keysym.sym;
      if (key == SDLK_ESCAPE)
      {
        ge->changeState(Title::Instance());
      }

      if (!highlight_enable)
      {
        highlight_enable = true;
        break;
      }

      if (key == SDLK_TAB)
      {
        SDL_Keymod keymod = SDL_GetModState();
        if (keymod == KMOD_LSHIFT || keymod == KMOD_RSHIFT || keymod == KMOD_SHIFT)
        {
          highlighted_selection--;
          if (highlighted_selection < 0)
            highlighted_selection = labels[highlighted_label].valid_selections - 1;
        }
        else
        {
          highlighted_selection++;
          if (highlighted_selection >= labels[highlighted_label].valid_selections)
            highlighted_selection = 0;
        }
      }
      else if (key == SDLK_RETURN || key == SDLK_SPACE)
      {
        if (highlight_enable)
          labels[highlighted_label].active_selection = highlighted_selection;
      }
      else if (key == SDLK_w || key == SDLK_UP)
      {
        highlighted_label--;
        if (highlighted_label < 0)
          highlighted_label = total_labels - 1;
        if (highlighted_selection >= labels[highlighted_label].valid_selections)
          highlighted_selection = labels[highlighted_label].valid_selections - 1;
      }
      else if (key == SDLK_s || key == SDLK_DOWN)
      {
        highlighted_label++;
        if (highlighted_label >= total_labels)
          highlighted_label = 0;
        if (highlighted_selection >= labels[highlighted_label].valid_selections)
          highlighted_selection = labels[highlighted_label].valid_selections - 1;

      }
      else if (key == SDLK_a || key == SDLK_LEFT)
      {
        highlighted_selection--;
        if (highlighted_selection < 0)
          highlighted_selection = labels[highlighted_label].valid_selections - 1;
      }
      else if (key == SDLK_d || key == SDLK_RIGHT)
      {
        highlighted_selection++;
        if (highlighted_selection >= labels[highlighted_label].valid_selections)
          highlighted_selection = 0;
      }
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN)
    {
      if (e.button.button == SDL_BUTTON_LEFT)
      {
        if (highlight_enable)
          labels[highlighted_label].active_selection = highlighted_selection;
      }
    }
    else if (e.type == SDL_MOUSEMOTION)
    {
      mouse_moved = true;
      mouseX = e.button.x;
      mouseY = e.button.y;
    }
  }
}

void Options::update()
{
  if (mouse_moved)
  {
    highlight_enable = false;
    for (int label_index = 0; label_index < 4; ++label_index)
    {
      for (int i = 0; i < labels[label_index].valid_selections; ++i)
      {
        Button b = labels[label_index].selection[i];
        if (checkPointRectCollision(mouseX, mouseY, b.rect))
        {
          highlighted_label = label_index;
          highlighted_selection = i;
          highlight_enable = true;
        }
      }
    }
    mouse_moved = false;
  }
}

void Options::render(GameEngine* ge)
{
  SDL_SetRenderDrawColor(ge->renderer, 0, 0, 0, 0);
  SDL_RenderClear(ge->renderer);

  SDL_RenderCopy(ge->renderer, title.texture, NULL, &title.rect);

  SDL_SetRenderDrawColor(ge->renderer, 0xff, 0xff, 0xff, 0);
  for (int label_index = 0; label_index < 4; ++label_index)
  {
    SDL_RenderCopy(ge->renderer, labels[label_index].texture, NULL, &labels[label_index].rect);
    for (int i = 0; i < labels[label_index].valid_selections; ++i)
    {
      Button b = labels[label_index].selection[i];
      SDL_RenderCopy(ge->renderer, b.texture, NULL, &b.rect);
      if (labels[label_index].active_selection == i)
        DrawButtonOutline(ge->renderer, b.rect, 10, 4);
    }
  }
  if (highlight_enable)
    DrawButtonOutline(ge->renderer, labels[highlighted_label].selection[highlighted_selection].rect, 5, 2);

  SDL_RenderPresent(ge->renderer);
}
