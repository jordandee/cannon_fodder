#ifndef RESOURCES_H
#define RESOURCES_H

#include <SDL2/SDL.h>

void loadSettings();
SDL_Texture* loadTexture(const char* filepath, SDL_Renderer* renderer);

#endif
