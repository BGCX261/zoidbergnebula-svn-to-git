#include <zenilib.h>
#include "MyPlayState.h"
#include "GameOverState.h"
#include "GameModel.h"
#include "GameObject.h"
#include "SharedPtrConfig.h"

using namespace Zeni;

MyPlayState::MyPlayState(int difficulty_) : prevTime(0), difficulty(difficulty_),
  gameModel(GameModel::getInstance()) {
  set_pausable(true);
  //gameModel->reset();
  SDL_WM_GrabInput(SDL_GRAB_ON);
  gameModel->reset();
  gameModel->setDifficulty(difficulty);
  initSkyBox();
}

MyPlayState::~MyPlayState() {
  SDL_WM_GrabInput(SDL_GRAB_OFF);
}

void MyPlayState::on_push() {
  
  get_Video().mouse_hide(true);
  Sound &sr = get_Sound();
  sr.set_BGM("sounds/FinalSacrifice.ogg" );
  sr.set_BGM_looping(true); // optional
  sr.play_BGM();
}

void MyPlayState::on_pop() {
  get_Sound().stop_BGM();
  get_Video().mouse_hide(false);
}

void MyPlayState::render() {
  Video &vr = get_Video();
  //vr.set_light(1, light);
  //vr.set_light(2, light2);
  //vr.set_lighting(true);
  //vr.set_ambient_lighting(Color(1, .8, .8, .8));
  vr.set_3d(gameModel->getPlayer().getCamera());
  renderBackground();
  gameModel->renderAll();
  
}

void MyPlayState::perform_logic() {
  if (gameModel->isGameOver()) {
    Game &game = get_Game();
    game.pop_state();
    game.push_state(new GameOverState());
  }
  const Time_HQ currTime = get_Timer_HQ().get_time();
  float processTime = float(currTime.get_seconds_since(prevTime));
  // 45 fps
  if (processTime < .022) {
    return;
  }
  prevTime = currTime;

  /** Keep delays under control (if the program hangs for some time, we don't want to lose responsiveness) **/
  if(processTime > 0.1f) {
    processTime = 0.1f;
  }

  gameModel->performAllLogic(processTime);
  /** Physics processing loop**
  for(float timeStep = 0.05f; processTime > 0.0f; processTime -= timeStep) {
    if(timeStep > processTime)
      timeStep = processTime;


    /** Try to move on each axis **
    partial_step(time_step, x_vel);
    partial_step(time_step, y_vel);
    partial_step(time_step, z_vel);
    */

    /** Keep player above ground; Bookkeeping for jumping controls /
    const Point3f &position = m_player.get_camera().position;
    if(position.z < 50.0f) {
      m_player.set_position(Point3f(position.x, position.y, 50.0f));
      m_player.set_on_ground(true);
    }*
  }*/
}

void MyPlayState::on_mouse_motion(const SDL_MouseMotionEvent &event) {
  gameModel->getPlayer().adjustPitch(event.yrel / 500.0f);
  gameModel->getPlayer().turnLeftXY(-event.xrel / 500.0f);
}

void MyPlayState::on_key(const SDL_KeyboardEvent &event) {
  
  switch(event.keysym.sym) {
    case SDLK_w:
      gameModel->getPlayer().setGoForward(event.type == SDL_KEYDOWN);
      break;
    case SDLK_s:
      gameModel->getPlayer().setGoBackward(event.type == SDL_KEYDOWN);
      break;
    case SDLK_a:
      gameModel->getPlayer().setBankLeft(event.type == SDL_KEYDOWN);
      break;    
    case SDLK_d:
      gameModel->getPlayer().setBankRight(event.type == SDL_KEYDOWN);
      break;
    case SDLK_q:
      gameModel->getPlayer().setGainAlt(event.type == SDL_KEYDOWN);
      break;    
    case SDLK_e:
      gameModel->getPlayer().setLoseAlt(event.type == SDL_KEYDOWN);
      break;
    case SDLK_LSHIFT:
	  case SDLK_RSHIFT:
      if (event.type == SDL_KEYDOWN) {
        gameModel->getPlayer().switchSupply();
      }
      break;
    case SDLK_SPACE:
      if (event.type == SDL_KEYDOWN) {
        gameModel->getPlayer().dropSupply();
      }
      break;
    default:
      Gamestate_Base::on_key(event);
  }

}

void MyPlayState::renderBackground() {
  Video &vr = get_Video();
  vr.render(floor);
  vr.render(side1);
  vr.render(side2);
  vr.render(side3);
  vr.render(side4);
  vr.render(sky);
}


void MyPlayState::initSkyBox() {

  Vertex3f_Texture p0(Point3f(-500, 500, 0), Point2f(0, 0)); 
  Vertex3f_Texture p1(Point3f(-500, -500, 0), Point2f(25, 0));
  Vertex3f_Texture p2(Point3f(500, -500, 0), Point2f(25, 25));
  Vertex3f_Texture p3(Point3f(500, 500, 0), Point2f(0, 25));
  Material material("grass");
  floor = Quadrilateral<Vertex3f_Texture>(p0, p1, p2, p3);
  floor.fax_Material(&material);

  p0 = Vertex3f_Texture(Point3f(-500, 500, 500), Point2f(0, 0)); 
  p1 = Vertex3f_Texture(Point3f(-500, -500, 500), Point2f(1, 0));
  p2 = Vertex3f_Texture(Point3f(500, -500, 500), Point2f(1, 1));
  p3 = Vertex3f_Texture(Point3f(500, 500, 500), Point2f(0, 1));
  Material material3("sky");
  sky = Quadrilateral<Vertex3f_Texture>(p0, p1, p2, p3);
  sky.fax_Material(&material3);

  p0 = Vertex3f_Texture(Point3f(-500, -500, 500), Point2f(0, 0));
  p1 = Vertex3f_Texture(Point3f(-500, -500, 0), Point2f(0, 1));
  p2 = Vertex3f_Texture(Point3f(500, -500, 0), Point2f(1, 1));
  p3 = Vertex3f_Texture(Point3f(500, -500, 500), Point2f(1, 0));
  Material material2("scary");
  side1 = Quadrilateral<Vertex3f_Texture>(p0, p1, p2, p3);
  side1.fax_Material(&material2);


  p0 = Vertex3f_Texture(Point3f(500, 500, 500), Point2f(0, 0));
  p1 = Vertex3f_Texture(Point3f(500, 500, 0), Point2f(0, 1));
  p2 = Vertex3f_Texture(Point3f(-500, 500, 0), Point2f(1, 1));
  p3 = Vertex3f_Texture(Point3f(-500, 500, 500), Point2f(1, 0));
  side2 = Quadrilateral<Vertex3f_Texture>(p0, p1, p2, p3);
  side2.fax_Material(&material2);
  
  p0 = Vertex3f_Texture(Point3f(-500, 500, 500), Point2f(0, 0));
  p1 = Vertex3f_Texture(Point3f(-500, 500, 0), Point2f(0, 1));
  p2 = Vertex3f_Texture(Point3f(-500, -500, 0), Point2f(1, 1));
  p3 = Vertex3f_Texture(Point3f(-500, -500, 500), Point2f(1, 0));
  side3 = Quadrilateral<Vertex3f_Texture>(p0, p1, p2, p3);
  side3.fax_Material(&material2);
  
  p0 = Vertex3f_Texture(Point3f(500, -500, 500), Point2f(0, 0));
  p1 = Vertex3f_Texture(Point3f(500, -500, 0), Point2f(0, 1));
  p2 = Vertex3f_Texture(Point3f(500, 500, 0), Point2f(1, 1));
  p3 = Vertex3f_Texture(Point3f(500, 500, 500), Point2f(1, 0));
  side4 = Quadrilateral<Vertex3f_Texture>(p0, p1, p2, p3);
  side4.fax_Material(&material2);

}
