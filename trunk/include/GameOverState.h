#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include <zenilib.h>
#include <set>

#include <iostream> 
#include <fstream> 

class GameOverState;

struct HighScore {
  HighScore(int score_, const std::string &name_) : score(score_), name(name_) {}
  HighScore(std::istream& is);
  int score;
  std::string name;
};

struct HighScoreCompare { 
  bool operator() (const HighScore& lhs, const HighScore& rhs) const{ 
    if (lhs.score != rhs.score) {
      return lhs.score > rhs.score; 
    } else {
      return lhs.name > rhs.name;
    }
  } 
}; 

/** Buttons **/
class MainMenu : public Zeni::Text_Button {
public:
  MainMenu(GameOverState* parentState_);

  void on_accept();
private:
  GameOverState* parentState;
}; 

class Quit : public Zeni::Text_Button {
public:
  Quit(GameOverState* parentState_);

  void on_accept();
private:
  GameOverState* parentState;
};

class SubmitScore : public Zeni::Text_Button {
public:
  SubmitScore(GameOverState* parentState_);

  void on_accept();
private:
  GameOverState* parentState;
};

/*class NameInput : public Zeni::Text_Box {
public:
  NameInput(GameOverState* parentState_);
private:
  GameOverState* parentState;
}*/

class GameOverState : public Zeni::Widget_Gamestate {
public:
  GameOverState();

  friend class MainMenu;
  friend class Quit;
  friend class SubmitScore;
private:
  void on_key(const SDL_KeyboardEvent &event);
  void render();
  void goToMainMenu();
  void quitGame();
  void importScores();
  void exportScores();
  void addScore();

  Zeni::Sound_Source* music;

  int playerScore;
  int rawScore;
  MainMenu menuButton;
  Quit quit;
  Zeni::Text_Box nameInput;
  SubmitScore submit;
  std::set<HighScore, HighScoreCompare> scores;
  bool scoreSubmitted; 
};

#endif
