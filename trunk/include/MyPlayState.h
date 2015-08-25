#ifndef MYPLAYSTATE_H
#define MYPLATSTATE_H

#include <zenilib.h>
#include "GameModel.h"
#include "Player.h"

class MyPlayState : public Zeni::Gamestate_Base {

public:
  MyPlayState(int difficulty_);
  ~MyPlayState();

private:
  void on_push();

  void on_pop();

  void render();

  void perform_logic();

  void on_key(const SDL_KeyboardEvent &event);

  void on_mouse_motion(const SDL_MouseMotionEvent &event);

  void renderBackground();

  void initSkyBox();
  /*******************Member variables**************************/
  GameModel *gameModel;
  Zeni::Time_HQ prevTime;
  int difficulty;
  Zeni::Quadrilateral<Zeni::Vertex3f_Texture> floor;
  Zeni::Quadrilateral<Zeni::Vertex3f_Texture> side1;
  Zeni::Quadrilateral<Zeni::Vertex3f_Texture> side2;
  Zeni::Quadrilateral<Zeni::Vertex3f_Texture> side3;
  Zeni::Quadrilateral<Zeni::Vertex3f_Texture> side4;
  Zeni::Quadrilateral<Zeni::Vertex3f_Texture> sky;
  //Player player;
};

#endif