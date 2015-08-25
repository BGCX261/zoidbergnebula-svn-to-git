#include <zenilib.h>
#include "GameOverState.h"
#include "GameModel.h"
//#include "Utility.h"

using namespace Zeni;
using std::set;
using std::getline;
using std::ifstream;
using std::ofstream;

typedef  set<HighScore, HighScoreCompare>::iterator setIter_t;

GameOverState::GameOverState()
      : Widget_Gamestate(std::make_pair(Point2f(0.0f, 0.0f), Point2f(1088, 612))),
      rawScore(GameModel::getInstance()->getRawScore()), playerScore(GameModel::getInstance()->getAdjustedScore()), menuButton(this), quit(this),
      nameInput(Point2f(800, 170), Point2f(1000, 220), "inGame", "Name", get_Colors()["yellow"], true, ZENI_CENTER),
      submit(this), scoreSubmitted(false)
  {
    get_Sound().stop_BGM();
    music = new Sound_Source(get_Sounds()["youSuck"]);
    get_Sound_Source_Pool().play_and_destroy(music);
    nameInput.lend_BG_Renderer(new Widget_Renderer_Color(Color(1.0f, 0.0f, 0.0f, 0.0f)));
    nameInput.set_editable(true);
    nameInput.set_focus(true);
    m_widgets.lend_Widget(menuButton);
    m_widgets.lend_Widget(quit);
    m_widgets.lend_Widget(nameInput);
    m_widgets.lend_Widget(submit);
    importScores();
}

void GameOverState::on_key(const SDL_KeyboardEvent &event) {
  if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED) {
    goToMainMenu();
  } else {
    Widget_Gamestate::on_key(event);
  }

}

void GameOverState::render() {
    Widget_Gamestate::render();
    float midScreen = get_Video().get_screen_width() / 2;
    Font &fontRender = get_Fonts()["title"];

    fontRender.render_text("Game Over!",
      Point2f(midScreen, 50.0f - 0.5f * fontRender.get_text_height()),
        get_Colors()["title_text"],
        ZENI_CENTER);

    Font &smallerFont = get_Fonts()["inGame"];

    smallerFont.render_text("Thanks for playing!",
        Point2f(170, 510),
        get_Colors()["white"],
        ZENI_CENTER);

    /*render_image(
      "ZoidM", // which texture to use
      Point2f(20, 200), // upper-left corner
      Point2f(320, 500), // lower-right corner
      0, //theta, // rotation in radians
      1, // scaling factor
      Point2f(), // point to rotate & scale about
      0, // whether or not to horizontally flip the texture
      Color()); // what Color to "paint" the texture*/
    GameModel *model = GameModel::getInstance();

    smallerFont.render_text("Your Raw Score was : " + itoa(rawScore),
        Point2f(20, 200),
        get_Colors()["white"],
        ZENI_LEFT);
    int numSaved = model->getNumSaved();
    int totalSurv = model->getTotalNumSurvivors();
    int totalZomb = model->getTotalNumZombies();
    int diff = model->getDifficulty();
    //playerScore = rawScore * ((numSaved + 1)/totalSurv * 1000) * totalZomb * diff * diff * 1000;
    smallerFont.render_text("Survivors Saved / Lost : " + itoa(numSaved) + " / " + itoa(totalSurv - numSaved),
        Point2f(20, 250),
        get_Colors()["white"],
        ZENI_LEFT);

    smallerFont.render_text("Number of Zombies faced : " + itoa(totalZomb),
        Point2f(20, 300),
        get_Colors()["white"],
        ZENI_LEFT);

    smallerFont.render_text("Difficulty Multiplier : " + itoa(diff * diff),
        Point2f(20, 350),
        get_Colors()["white"],
        ZENI_LEFT);

    smallerFont.render_text("Final Score : " + itoa(playerScore),
        Point2f(20, 400),
        get_Colors()["yellow"],
        ZENI_LEFT);

    smallerFont.render_text("High Scores",
        Point2f(525, 150),
        get_Colors()["yellow"],
        ZENI_CENTER);
    int yCoords = 180;
    for (setIter_t it = scores.begin(); it != scores.end(); it++, yCoords+=40) {
      smallerFont.render_text((*it).name + " ---- " + itoa((*it).score),
        Point2f(650, yCoords),
        get_Colors()["white"],
        ZENI_RIGHT);  
    }
}

void GameOverState::goToMainMenu() {
  music->stop();
  exportScores();
  get_Game().pop_state();
}

void GameOverState::quitGame() {
  exportScores();
  get_Game().pop_state();
  get_Game().pop_state();
}

void GameOverState::importScores() {
  ifstream file; 
  file.open("docs/HighScores.txt"); 
  try {
    if(!file.is_open()){ 
      //throw MyError("Could not open file!"); 
    } 
    //
    for (int i = 0; i < 10; i++) {
      HighScore score(file);
      scores.insert(score);
    }
  } catch (...) {

  }
  file.close();
}

void GameOverState::exportScores(){
  ofstream file;
  file.open("docs/HighScores.txt"); 
  try {
    if(!file.is_open()){ 
      //throw MyError("Could not open file!"); 
    } 
    for (setIter_t it = scores.begin(); it != scores.end(); it++) {
      file << (*it).score << (*it).name << "\n";
    }
  } catch (...) {

  }
  file.close();
}

void GameOverState::addScore(){
  if (!scoreSubmitted) {
    scores.insert(HighScore(playerScore, " " + nameInput.get_text()));
    scoreSubmitted = true;
    nameInput.set_editable(false);
    nameInput.set_focus(false);
  }
}

void MainMenu::on_accept() {
  parentState->goToMainMenu();
}


MainMenu::MainMenu(GameOverState* parentState_)  :
  Text_Button(Point2f(get_Video().get_screen_width() - 250, get_Video().get_screen_height() - 50),
  Point2f(get_Video().get_screen_width() - 10, get_Video().get_screen_height() - 10),
    "system_36_800x600", "Main Menu"), parentState(parentState_)
  {
  }

void Quit::on_accept() {
  parentState->quitGame();
}


Quit::Quit(GameOverState* parentState_)  :
Text_Button(Point2f(get_Video().get_screen_width() - 400, get_Video().get_screen_height() - 50),
  Point2f(get_Video().get_screen_width() - 260, get_Video().get_screen_height() - 10),
    "system_36_800x600", "Quit"), parentState(parentState_)
  {
  }

SubmitScore::SubmitScore(GameOverState* parentState_) :
  Text_Button(Point2f(800, 270), Point2f(1000, 320),
    "system_36_800x600", "Submit Score"), parentState(parentState_)
  {
  }

void SubmitScore::on_accept() {
  get_Sound_Source_Pool().play_and_destroy(new Sound_Source(get_Sounds()["zombieMoan"]));
  parentState->addScore();
}

HighScore::HighScore(std::istream& is) {
  if(!(is >> score)) {
    //throw MyError("Error Importing High Scores");
  }
  if (!getline(is, name)) {
    //throw MyError("SIgh");
  }
}
