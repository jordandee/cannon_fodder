#ifndef LEVEL_H
#define LEVEL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "gamestate.h"
#include "terrain.h"
#include "timer.h"
#include "cannon.h"
#include "ball.h"
#include "score.h"
#include "obstacle.h"

class Level : public GameState
{
public:
  void init(GameEngine* ge);
  void quit();

  void handleEvents(GameEngine* ge);
  void update();
  void render(GameEngine* ge);

  void spawnLevel();

  static Level* Instance() { return &_s; }

protected:
  Level() { }

private:
  static Level _s;

  std::vector<Pixel> terrain;
  std::vector<Obstacle> obstacles;

  Cannon cannonL, cannonR;

  Ball ball;

  Timer timer, shot_timer, respawn_timer;
  double dt;
  double shot_dt;
  bool shooting, shot_live;
  int keyup_frames;

  SDL_Texture* force_texture;
  SDL_Rect force_rect;

  bool is_player1, is_a_player_dead;

  Score score;
  int win_score;
  
  int missed_left;
  int missed_right;
  int missed_distance;
  double last_ai_angle;
  double last_ai_shot_dt;
};

#endif
