#ifndef RESOURCES_H
#define RESOURCES_H

#include <SDL2/SDL.h>

void loadSettings();
void saveSettings();
void setupSettingsBasedGlobals();

SDL_Texture* loadTexture(const char* filepath, SDL_Renderer* renderer);

#endif
