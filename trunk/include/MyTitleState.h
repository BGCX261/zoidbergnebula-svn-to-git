#ifndef MYTITLESTATE_H
#define MYTITLESTATE_H

#include <zenilib.h>

class Title : public Zeni::Text_Box {
public:
  Title(const std::string &title_);
};

class EasyButton : public Zeni::Text_Button {
public:
  EasyButton();

  void on_accept();
};

class MediumButton : public Zeni::Text_Button {
public:
  MediumButton();

  void on_accept();
};

class HardButton : public Zeni::Text_Button {
public:
  HardButton();

  void on_accept();
};

class InstructionsButton : public Zeni::Text_Button {
public:
  InstructionsButton();
  void on_accept();
};



class MyTitleState : public Zeni::Widget_Gamestate {
public:
  MyTitleState();
  ~MyTitleState();
private:
  Title title;
  EasyButton easy;
  MediumButton medium;
  HardButton hard;
  InstructionsButton instr;
  Zeni::Popup_Menu_State::Configure_Video_Button configureVid;
  Zeni::Popup_Menu_State::Sound_Check_Box soundCheckBox;
  Zeni::Popup_Menu_State::Quit_Button quitButton;
  void perform_logic();
  void render();
  void on_push();
  void on_pop();

  Zeni::Sound_Source* sound;
};

#endif