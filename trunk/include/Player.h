#ifndef PLAYER_H
#define PLAYER_H

#include <zenilib.h>
#include "GameObject.h"
#include "SharedPtrConfig.h"
#include "Supply.h"

#include <map>
#include <vector>

class Player : public GameObject {
public:
  Player();
  ~Player();

  const Zeni::Camera & getCamera() const {
    return camera;
  }

  const Zeni::Collision::Sphere getBody() { 
    return body;
  }

  void render();

  void performLogic(float timeDelta);

  void adjustPitch(const float &phi);
  void turnLeftXY(const float &theta);

  void setGoForward(bool set);
  void setGoBackward(bool set);
  void setBankLeft(bool set);
  void setBankRight(bool set);
  void setGainAlt(bool set) {
    up = set;
  }
  void setLoseAlt(bool set) {
    down = set;
  }

  const Supply & getSelectedSuppply() {
   return selectedSupply;
  }

  void addSupply(); 

  void switchSupply();

  void dropSupply();

  void setSite(SurvivalSite_sp_t site_) {
    site = site_;
  }

  void initValues(int difficulty);

private:
  Zeni::Camera camera; 
  Zeni::Model model;

  // Object properties
  Zeni::Point3f corner;
  Zeni::Vector3f scale;
  Zeni::Quaternion rotation;
  Zeni::Collision::Sphere body;

  // Movements
  bool forward;
  bool backward;
  bool left;
  bool right;
  bool up;
  bool down;
  int animation;
  Supply selectedSupply;

  SurvivalSite_sp_t site;

  void updateBody();
  void renderSelectedSupply();

  std::map<SupplyTypes, std::vector<Supply> > supplies;
  std::tr1::shared_ptr<Zeni::Sound_Source> heliSound;
};

#endif 