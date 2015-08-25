#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <zenilib.h>

/**
 * Game object class, used for all objects in a game
 */
class GameObject : public std::tr1::enable_shared_from_this<GameObject> {
public:
  GameObject() : isZombie(false){ 
    id = nextId++;
  }
  virtual ~GameObject(){}

  virtual void render() = 0; 
  virtual void performLogic(float timeDelta) = 0;


  int getId() const {
    return id;
  }

  bool isZombieObj() {
    return isZombie;
  }

  void setZombie() {
    isZombie = true;
  }


private:
  bool isZombie;
  static int nextId;
  int id;
};

#endif
