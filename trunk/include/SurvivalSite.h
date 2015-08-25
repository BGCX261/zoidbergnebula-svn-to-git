#ifndef SURVIVALSITE_H
#define SURVIVALSITE_H

#include <zenilib.h>
#include "GameObject.h"
#include "Supply.h"

class SurvivalSite : public GameObject {
public:
  SurvivalSite(std::tr1::shared_ptr<Zeni::Model> outpost, const Zeni::Point3f &position_);
  ~SurvivalSite() {}

  virtual void render();
  virtual void performLogic(float timeDelta);

  const Zeni::Collision::Infinite_Cylinder & getBody() {
    return body;
  }

  virtual void addSupplies(const Supply &supply);

  virtual bool requestSupply(enum SupplyTypes type) {
    return false;
  }

  virtual bool isBase() {
    return false;
  }

private:
  float health;
  int numSurvivors;
  int numZombies;
  float supplies;
  float timeUntilZombie; 
  int animation;
  float zombieMovement;

  bool allSaved;
  bool allDead;
  bool setValues;

  std::tr1::shared_ptr<Zeni::Model> model;
  Zeni::Point3f position;

  void initValues();
  void updateHealth(float timeDelta);
  void renderHealthBar();
  void renderImage();

  Zeni::Collision::Infinite_Cylinder body;
  Zeni::Vector3f survivorScale;
  Zeni::Vector3f zombieScale;

};

#endif
