#include <zenilib.h>
#include "SurvivalSite.h"
#include "GameModel.h"

using namespace Zeni;


SurvivalSite::SurvivalSite(std::tr1::shared_ptr<Model> outpost, const Zeni::Point3f &position_) 
  : model(outpost), position(position_), health(100), animation(0), zombieMovement(0), allSaved(false), allDead(false), setValues(false) {
  initValues();
  body = Collision::Infinite_Cylinder(position, Vector3f(0,0,1000), 25);
}

void SurvivalSite::render() {
  model->set_translate(position);
  model->render();
 // body = Collision::Infinite_Cylinder(position, Vector3f(0,0,1000), 50);
  if (numSurvivors) {
    std::tr1::shared_ptr<Zeni::Model> survivor = GameModel::getInstance()->getSurvivorModel();
    animation = animation <= 100 ? animation + 1 : 0;
    Point3f cameraPos = GameModel::getInstance()->getPlayer().getCamera().position;
    Vector3f camera(cameraPos);
    camera.x -= position.x;
    camera.y -= position.y;
    camera.z = 0;
    float angle = camera.angle_between(position);
    Quaternion rot = Quaternion::Axis_Angle(Vector3f(0,0,1), -angle);
    survivor->set_rotate(rot);
    survivor->set_scale(survivorScale);
    survivor->set_keyframe(animation);
    survivor->set_translate(position);
    if (allDead) {
 
      survivor->set_rotate(Quaternion::Axis_Angle(Vector3f(-1,-1,0), pi/4));
      survivor->set_keyframe(35);
      survivor->set_scale(Vector3f(1,1,1));
      survivor->set_translate(position - Point3f(-10, 0, 0));
    }
   
    survivor->render();
  }
  std::tr1::shared_ptr<Zeni::Model> zombie = GameModel::getInstance()->getZombieModel();
  zombieMovement = zombieMovement < 100 ? zombieMovement + .5 : 0;
  zombie->set_keyframe(static_cast<int>(zombieMovement));
  zombie->set_scale(zombieScale);
  zombie->set_translate(position - Point3f(-15, -80, 0));
  zombie->set_rotate(Quaternion());
  if (allDead) {
    zombie->set_translate(position - Point3f(0, 0, 0));
    zombie->set_rotate(Quaternion::Axis_Angle(Vector3f(0,0,1), pi));
    zombie->set_scale(Vector3f(1,1,1));
  }
  zombie->render();
  if (allSaved || allDead) {
    renderImage();
  } else {
    renderHealthBar();
  }
  
}

void SurvivalSite::performLogic(float timeDelta) {
  updateHealth(timeDelta);
  if (allDead || allSaved) {
    return;
  }
  timeUntilZombie -= timeDelta;
  if (timeUntilZombie < 0 ) {
    numZombies++;
    timeUntilZombie = rand() % 20 + 30 - GameModel::getInstance()->getDifficulty() * 2;
    zombieScale *=  1.1 ;
    if (zombieScale.x > 2.5) {
      zombieScale = Vector3f(2.5,2.5,2.5);
    }
    Sound_Source *sound = new Sound_Source(get_Sounds()["zombieMoan"]);
    sound->set_position(position);
    sound->set_gain(3);
    get_Sound_Source_Pool().play_and_destroy(sound);
    GameModel::getInstance()->addTotalNumZombies(1);
  }
  if (!setValues) {
    GameModel::getInstance()->addNumTotalSurivors(numSurvivors);
    GameModel::getInstance()->addTotalNumZombies(numZombies);
    setValues = true;
  }
}

void SurvivalSite::initValues() {
  health = 100;
  numSurvivors = rand() % 15 + 10;
  numZombies = 10;//rand() % 5 + 1;
  supplies = 1;
  timeUntilZombie = rand() % 20 + 30; 
  survivorScale = Vector3f(1,1,1) * numSurvivors/10;
  zombieScale = Vector3f(1,1,1) * numZombies/5;

}

void SurvivalSite::updateHealth(float timeDelta) {
  if (allDead || allSaved) {
    return;
  }
  int difficutly = GameModel::getInstance()->getDifficulty();
  float healthDelta = (numZombies * ( 3 * difficutly) - numSurvivors * supplies) * difficutly / 2;
  health = health <= 100 ? health - healthDelta * timeDelta / 100 : 100;
  supplies = supplies < 1 ? 1 : supplies - (timeDelta/7 * difficutly / 2) ;
  /*if (numSurvivors && rand() % static_cast<int>(1 / (numSurvivors * supplies) * 100)) {
    numZombies--;
    timeUntilZombie--;
    get_Sound_Source_Pool().play_and_destroy(new Sound_Source(get_Sounds()["scream"]));
  }*/
   if (health <= 0) {
    numSurvivors -= 5;
    health = 50 / difficutly;
    survivorScale = numSurvivors > 5 ? Vector3f(1,1,1) * numSurvivors/10 : Vector3f(.5,.5,.5);
  }
  if (numSurvivors <= 0) {
    allDead = true;
    health = 0;
    setZombie();
  }
}

void SurvivalSite::renderHealthBar() {
  get_Video().set_2d(
    std::pair<Point2f, Point2f>(Point2f(0,0),  get_Video().get_screen_size()), true);
  get_Video().set_ztest(false);
  get_Video().set_zwrite(false);
  Projector3D projector(GameModel::getInstance()->getPlayer().getCamera(),
    std::make_pair(Point2i(0,0), get_Video().get_screen_size()));
  Point3f point = projector.project(position);
  if (point.z > 0) {
    render_image(
        "border", // which texture to use
        Point2f(point.x - 50, point.y + 10), // upper-left corner
        Point2f(point.x + 50, point.y + 20), // lower-right corner
        0, //theta, // rotation in radians
        1.0f, // scaling factor
        Point2f(0,0), // point to rotate & scale about
        false, // whether or not to horizontally flip the texture
        Color()); // what Color to "paint" the texture
     render_image(
        "red", // which texture to use
        Point2f(point.x - 50, point.y + 10), // upper-left corner
        Point2f(point.x - 50 + health, point.y + 20), // lower-right corner
        0, //theta, // rotation in radians
        1.0f, // scaling factor
        Point2f(0,0), // point to rotate & scale about
        false, // whether or not to horizontally flip the texture
        Color()); // what Color to "paint" the texture
  }

  get_Video().set_3d(GameModel::getInstance()->getPlayer().getCamera());
  get_Video().set_ztest(true);
  get_Video().set_zwrite(true);
}

void SurvivalSite::addSupplies(const Supply &supply) {
  if (allDead || allSaved) {
    // what a waste
    return;
  }
  if (supply.type == RESCUE) {
    numSurvivors--;
    survivorScale = numSurvivors > 5 ? Vector3f(1,1,1) * numSurvivors/10 : Vector3f(.5,.5,.5);
    GameModel::getInstance()->addNumSaved(1);
    if (numSurvivors == 0) {
      allSaved = true;
      setZombie();
    }
  }
  supplies += supply.value;
  get_Sound_Source_Pool().play_and_destroy(new Sound_Source(get_Sounds()[supply.soundName]));
}

void SurvivalSite::renderImage() {
  Video &vr = get_Video();
  int xCoord = position.x;
  int yCoord = position.y;
  Vertex3f_Texture p0(Point3f(xCoord + 50, yCoord + 50, 1), Point2f(0, 0)); 
  Vertex3f_Texture p1(Point3f(xCoord + 50, yCoord + -50, 1), Point2f(1, 0));
  Vertex3f_Texture p2(Point3f(xCoord + -50, yCoord + -50, 1), Point2f(1, 1));
  Vertex3f_Texture p3(Point3f(xCoord + -50, yCoord + 50, 1), Point2f(0, 1));
  Material material(allSaved ? "happyFace" : "Skull");
  Quadrilateral<Vertex3f_Texture> quad(p0, p1, p2, p3);
  quad.fax_Material(&material);
  vr.render(quad);

}