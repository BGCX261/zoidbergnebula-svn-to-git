#include <zenilib.h>
#include "MyTitleState.h"
#include "MyPlayState.h"
#include "MyInstructionState.h"

using namespace Zeni;


MyTitleState::MyTitleState()
      : Widget_Gamestate(std::make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f))),
      title("stranDEAD"),
      configureVid(Point2f(200.0f, 410.0f), Point2f(600.0f, 470.0f)),
      soundCheckBox(Point2f(200.0f, 490.0f), Point2f(260.0f, 550.0f)),
      quitButton(Point2f(400.0f, 490.0f), Point2f(600.0f, 550.0f))
{
  m_widgets.lend_Widget(title);
  m_widgets.lend_Widget(easy);
  m_widgets.lend_Widget(medium);
  m_widgets.lend_Widget(hard);
  m_widgets.lend_Widget(instr);
  m_widgets.lend_Widget(configureVid);
  m_widgets.lend_Widget(soundCheckBox);
  m_widgets.lend_Widget(quitButton);

  title.set_justify(ZENI_CENTER);

  get_Video().set_clear_color(get_Colors()["title_bg"]);
}

MyTitleState::~MyTitleState() {
  get_Video().set_clear_color(Color(1.0f, 0.0f, 0.0f, 0.0f));
}


void MyTitleState::perform_logic() {
  Widget_Gamestate::perform_logic();
  get_Video().set_clear_color(get_Colors()["title_bg"]);
}

void MyTitleState::render() {
  int height = get_Video().get_screen_height();
  int width = get_Video().get_screen_width();
  /*render_image(
      "background", // which texture to use
      Point2f(0,0), // upper-left corner
      Point2f(width,height), // lower-right corner
      0, //theta, // rotation in radians
      1.0f, // scaling factor
      Point2f(0,0), // point to rotate & scale about
      false, // whether or not to horizontally flip the texture
      Color()); // what Color to "paint" the texture*/
  soundCheckBox.set_checked(!get_Sound_Source_Pool().is_muted());
  Widget_Gamestate::render();
}

void MyTitleState::on_push() {
  /*Sound &sr = get_Sound();
  sr.set_BGM("sounds/ValkriesSnippet.ogg" );
  sr.set_BGM_looping(true); // optional
  sr.play_BGM();*/
}

void MyTitleState::on_pop() {
  //get_Sound().stop_BGM();
}

/*************BUUTTONSSS********/

Title::Title(const std::string &title_)
    : Text_Box(Point2f(000.0f, 50.0f), Point2f(800.0f, 250.0f),
                "title", title_, get_Colors()["title_text"])
{
    give_BG_Renderer(new Widget_Renderer_Color(Color(0.0f, 0.0f, 0.0f, 0.0f)));
}

EasyButton::EasyButton()
    : Text_Button(Point2f(150.0f, 250.0f), Point2f(300, 310.0f),
                  "system_36_800x600", "Laird")
{
}

void EasyButton::on_accept() {
  get_Game().push_state(new MyPlayState(1));
}

MediumButton::MediumButton() :
  Text_Button(Point2f(325.0f, 250.0f), Point2f(475.0f, 310.0f),
  "system_36_800x600", "Mortal"){}

void MediumButton::on_accept(){
  get_Game().push_state(new MyPlayState(2));
}

HardButton::HardButton() :
  Text_Button(Point2f(500.0f, 250.0f), Point2f(650.0f, 310.0f),
  "system_36_800x600", "Romero"){}

void HardButton::on_accept(){
  get_Game().push_state(new MyPlayState(3));
}

InstructionsButton::InstructionsButton()
    : Text_Button(Point2f(200.0f, 330.0f), Point2f(600.0f, 390.0f),
                  "system_36_800x600", "Instructions")
  {
  }

void InstructionsButton::on_accept() {
  get_Game().push_state(new MyInstructionState());
}