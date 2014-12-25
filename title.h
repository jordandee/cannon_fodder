#ifndef TITLE_H
#define TITLE_H

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
};

#endif
