#ifndef MYINSTRSTATE_H
#define MYINSTRSTATE_H

#include <zenilib.h>

class BackToMenu : public Zeni::Text_Button {
public:
  BackToMenu();
  void on_accept();
};

class MyInstructionState : public Zeni::Widget_Gamestate {

public:
  MyInstructionState();

private:
  void on_key(const SDL_KeyboardEvent &event);
  void render();
  BackToMenu menu;
};
#endif