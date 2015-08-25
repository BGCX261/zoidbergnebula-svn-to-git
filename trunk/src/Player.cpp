#include <zenilib.h>

#include "Player.h"
#include "GameModel.h"

#include <string>

using namespace Zeni;

using std::string;

Player::Player() : camera(Point3f(0.0f, -100.0f, 175.0f),
             Quaternion(), 1.0f, 10000.0f), model("models/moreSuperStrippedHeli.3ds"), forward(false),
             backward(false), left(false), right(false), up(false), down(false), animation(0), site(NULL), selectedSupply(BAT),
             heliSound(new Sound_Source(get_Sounds()["heli"])) {
  model.set_translate(Point3f(0,0,50));
  rotation = Quaternion::Axis_Angle(Vector3f(0,0,1), -1 * pi/2);
  updateBody();
  heliSound->set_looping(true);
  heliSound->play();
}

void Player::initValues(int difficulty) {
  for( int i = 0 ; i < 10 / difficulty; i++) {
    supplies[BAT].push_back(Supply(BAT));
  }
  for( int i = 0 ; i < 5  / difficulty; i++) {
    supplies[PISTOL].push_back(Supply(PISTOL));
  }
  for( int i = 0 ; i < 3 / difficulty ; i++) {
    supplies[RIFLE].push_back(Supply(RIFLE));
  }
  for( int i = 0 ; i < 2 / difficulty; i++) {
    supplies[RESCUE].push_back(Supply(RESCUE));
  }
}

void Player::render() {
  animation = animation > 9? 0 : animation + 1;
  //model.set_translate(Point3f(10,10,10));
  model.set_scale(Vector3f(1,1,1));
  model.set_rotate(rotation);
  model.set_keyframe(animation);
  model.render();

  renderSelectedSupply();
}


void Player::adjustPitch(const float &phi) {
  const Quaternion backup = camera.orientation;
  const Vector3f backup_up = camera.get_up();

  camera.adjust_pitch(phi);

  if(camera.get_up().k < 0.0f && backup_up.k >= 0.0f)
    camera.orientation = backup;
}

void Player::turnLeftXY(const float &theta) {

  rotation = rotation * Quaternion::Axis_Angle(Vector3f(0,0,1), theta);
  camera.turn_left_xy(theta);
}

void Player::performLogic(float timeDelta) {
  //Quaternion(yaw, pitch, roll);
   //rotation *= Quaternion(0,.01,0);
  //model.set_rotate(Quaternion(0, 10, 0));
  if (GameModel::getInstance()->isGameOver()) {
    heliSound->stop();
  }
    /** Get forward and left vectors in the XY-plane **/
  const Vector3f forwardDir = camera.get_forward().get_ij().normalized();
  const Vector3f leftDir = camera.get_left().get_ij().normalized();

  /** Get velocity vector split into a number of axes **/
  const Vector3f velocity = (forward - backward) * 90.0f * forwardDir
                          + (left - right) * 90.0f * leftDir 
                          + (down - up) * 50 * Vector3f(0,0,1);
  const Vector3f x_vel = velocity.get_i();
  const Vector3f y_vel = velocity.get_j();
  Vector3f z_vel = velocity.get_k();

  camera.position += timeDelta * velocity;
  
  if (camera.position.z < 175) {
    camera.position = Point3f(camera.position.x, camera.position.y, 175);
  } else if (camera.position.z > 400) {
    camera.position = Point3f(camera.position.x, camera.position.y, 400);
  }

  if (camera.position.x > 400) {
    camera.position = Point3f(400, camera.position.y, camera.position.z);
  } else if (camera.position.x  < -400 ) {
    camera.position = Point3f(-400, camera.position.y, camera.position.z);
  }

  if (camera.position.y > 400) {
    camera.position = Point3f(camera.position.x, 400, camera.position.z);
  } else if (camera.position.y  < -400 ) {
    camera.position = Point3f(camera.position.x, -400, camera.position.z);
  }

  Point3f backupPos = camera.position + forwardDir * 75;
  Point3f newPos = camera.position + forwardDir * 75;

  if (!(forwardDir.x < .005 && forwardDir.x > -.005 && forwardDir.y < .005 && forwardDir.y > -.005)) {
    model.set_translate(newPos - Point3f(0,0,50));
  }
  updateBody();
}

void Player::switchSupply() {
  switch(selectedSupply.type) {
    case BAT:
      selectedSupply = Supply(PISTOL);
      break;
    case PISTOL:
      selectedSupply = Supply(RIFLE);
      break;
    case RIFLE:
      selectedSupply = Supply(RESCUE);
      break;
    case RESCUE:
      selectedSupply = Supply(BAT);
      break;
    case SURVIVOR:
      break;
    default:
      break;
  }
}

void Player::dropSupply() {
  if (!site.get()) {
    return;
  }

  if (site->isBase()) {
    if (site->requestSupply(selectedSupply.type)) {
      supplies[selectedSupply.type].push_back(Supply(selectedSupply.type));
      GameModel::getInstance()->addToScore(selectedSupply.cost);
    } else {
      get_Sound_Source_Pool().play_and_destroy(new Sound_Source(get_Sounds()["cantDoIt"]));
    }
  } 
  
  if (site->isZombieObj()) {
    get_Sound_Source_Pool().play_and_destroy(new Sound_Source(get_Sounds()["noOneLeft"]));
    return;
  }

  if (!site->isBase() && supplies[selectedSupply.type].size() > 0) {
    //get_Sound_Source_Pool().play_and_destroy(new Sound_Source(get_Sounds()["badFeeling"]));
    site->addSupplies(supplies[selectedSupply.type][0]);
    supplies[selectedSupply.type].pop_back();
    GameModel::getInstance()->addToScore(selectedSupply.cost);
  } 
}

void Player::updateBody() {
  body = Collision::Sphere(model.get_translate(), 50);
}

void Player::setGoForward(bool set){
  forward = set;
}

void Player::setGoBackward(bool set){
  backward = set;
}

void Player::setBankLeft(bool set){
  left = set;
}

void Player::setBankRight(bool set){
  right = set;
}

void Player::renderSelectedSupply() {
 
  get_Video().set_2d(
    std::pair<Point2f, Point2f>(Point2f(0,0),  get_Video().get_screen_size()), true);
  get_Video().set_ztest(false);
  get_Video().set_zwrite(false);
  
  render_image(
    selectedSupply.imgName, // which texture to use
      Point2f(0, 0), // upper-left corner
      Point2f(100, 100), // lower-right corner
      0, //theta, // rotation in radians
      1.0f, // scaling factor
      Point2f(0,0), // point to rotate & scale about
      false, // whether or not to horizontally flip the texture
      Color()); // what Color to "paint" the texture
  Zeni::Font &fontRender = get_Fonts()["inGame"];
  fontRender.render_text(itoa(supplies[selectedSupply.type].size()),  Point2f(100, 70),
                  get_Colors()["white"],
                  ZENI_RIGHT);
  get_Video().set_3d(GameModel::getInstance()->getPlayer().getCamera());
  get_Video().set_ztest(true);
  get_Video().set_zwrite(true);
}

Player::~Player() {

}

