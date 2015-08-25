#include <zenilib.h>
#include "MyInstructionState.h"

using namespace Zeni;
using std::make_pair;

MyInstructionState::MyInstructionState() :
  Widget_Gamestate(make_pair(Point2f(0.0f, 0.0f), Point2f(1088, 612)))
  {
    m_widgets.lend_Widget(menu);
  }

void MyInstructionState::on_key(const SDL_KeyboardEvent &event) {
  if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED)
    get_Game().pop_state();
}

void MyInstructionState::render() {
    Widget_Gamestate::render();
    float midScreen = get_Video().get_screen_width() / 2;
    Font &fontRender = get_Fonts()["title"];

    fontRender.render_text("Instructions",
      Point2f(midScreen, 50.0f - 0.5f * fontRender.get_text_height()),
        get_Colors()["title_text"],
        ZENI_CENTER);

    Font &smallerFont = get_Fonts()["instr"];
    smallerFont.render_text("You'll be in control of a Helicopter, your mission, should you choose to accept it,",
        Point2f(midScreen, 100),
        get_Colors()["white"],
        ZENI_CENTER);
    smallerFont.render_text("Is to resupply and rescue groups of survivors that have managed to fortify themselves.",
        Point2f(midScreen, 130),
        get_Colors()["yellow"],
        ZENI_CENTER);
     smallerFont.render_text("They won't last long without your help!",
        Point2f(midScreen, 160),
        get_Colors()["white"],
        ZENI_CENTER);
    smallerFont.render_text("You can deliver four different supplies. Weapons: Cricket Bats, Pistols, Automatic Rifles,",
        Point2f(midScreen, 190),
        get_Colors()["yellow"],
        ZENI_CENTER);
    smallerFont.render_text("These buy your survivors precious time, and can even give them back some health (Shown with a red bar).",
        Point2f(midScreen, 220),
        get_Colors()["white"],
        ZENI_CENTER);
	smallerFont.render_text("The fourth, but most important, is the life ring. Tossing one of these to a group will rescue one survivor",
        Point2f(midScreen, 250),
        get_Colors()["yellow"],
        ZENI_CENTER);
	smallerFont.render_text("Rescuing survivors early is important The more you've rescued the more supplies your base will produce!",
        Point2f(midScreen, 280),
        get_Colors()["white"],
        ZENI_CENTER);
	smallerFont.render_text("You can use SHIFT to switch which supply to drop/request. Your selected item will be on the top left of your screen",
        Point2f(midScreen, 310),
        get_Colors()["yellow"],
        ZENI_CENTER);
	smallerFont.render_text("You can use the SPACE BAR while hovering over a survial site to drop your selected supply",
        Point2f(midScreen, 340),
        get_Colors()["white"],
        ZENI_CENTER);
  smallerFont.render_text("To resupply, simply hover over you base, use SHIFT to select the desired supply type and press SPACE BAR to accept",
        Point2f(midScreen, 370),
        get_Colors()["yellow"],
        ZENI_CENTER);
	smallerFont.render_text("If your base can provide the supply, its name will show in green below the blue \"purchase points\" bar",
        Point2f(midScreen, 400),
        get_Colors()["white"],
        ZENI_CENTER);
  smallerFont.render_text("Use W A S D to move in the four cardinal directions, Q to move down, E to move up and Your mouse to look around.",
        Point2f(midScreen, 430),
        get_Colors()["yellow"],
        ZENI_CENTER);
	smallerFont.render_text("Each Survivor group is represented by one zombie and one survivor. They're scaled proportional to the amount of zombies/survivors.",
        Point2f(midScreen, 460),
        get_Colors()["white"],
        ZENI_CENTER);
  smallerFont.render_text("When choosing who to resupply, its important to consider the ration of zombies to survivors (their size).",
        Point2f(midScreen, 490),
        get_Colors()["yellow"],
        ZENI_CENTER);
	smallerFont.render_text("Resupplying groups with larger amounts of survivors will have a greater effect. Sites with large amounts of zombies will not last as long.",
        Point2f(midScreen, 520),
        get_Colors()["white"],
        ZENI_CENTER);
  smallerFont.render_text("Good Luck!",
        Point2f(midScreen, 550),
        get_Colors()["yellow"],
        ZENI_CENTER);
   
  }

BackToMenu::BackToMenu() :
  Text_Button(Point2f(get_Video().get_screen_width() - 250, get_Video().get_screen_height() - 50),
  Point2f(get_Video().get_screen_width() - 10, get_Video().get_screen_height() - 10),
    "system_36_800x600", "Main Menu")
  {
  }

void BackToMenu::on_accept() {
  get_Game().pop_state();
}
