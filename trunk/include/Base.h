#ifndef BASE_H
#define BASE_H

#include <zenilib.h>
#include "SurvivalSite.h"

class Base : public SurvivalSite {
public:
  Base(std::tr1::shared_ptr<Zeni::Model> outpost);
  ~Base() {}

  void render();
  void performLogic(float timeDelta);

  const Zeni::Collision::Infinite_Cylinder & getBody() {
    return body;
  }

  bool requestSupply(enum SupplyTypes type);

  bool isBase() {
    return true;
  }

private:
  std::tr1::shared_ptr<Zeni::Model> model;
  Zeni::Point3f position;
  Zeni::Collision::Infinite_Cylinder body;

  float purchasePoints;
  int multiplier; 
  Zeni::Quadrilateral<Zeni::Vertex3f_Texture> pad;
};

#endif
