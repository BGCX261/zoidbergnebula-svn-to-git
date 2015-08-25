#ifndef MODEL_H
#define MODEL_H

#include <zenilib.h>
#include "SharedPtrConfig.h"
#include "Player.h"
#include "SurvivalSite.h"
#include "Base.h"

#include <vector>

/**
 Singleton class that holds the data for the game
*/
class GameModel {
public:
  //singleton getinstance, creates 
  static GameModel* getInstance();
	// return the current time
	float getTimeDelta();

  //friend class so we can destruct this
  friend class GameModelDestroyer;

  Player & getPlayer() {
    return player;
  }

  void performAllLogic(float timeDelta);
  void renderAll();

   std::tr1::shared_ptr<Zeni::Model> getZombieModel() {
     return zombie;
   }

   std::tr1::shared_ptr<Zeni::Model> getSurvivorModel() {
     return survivor;
   }

   void addNumTotalSurivors(int add) {
     totalNumSurvivors += add;
   }

   void addNumSaved(int add) {
     numSaved += add;
     multiplier += ((27 - difficulty * 2) / numSaved) ;
   }

   void addTotalNumZombies(int add) {
     totalNumZombies += add;
   }

   float getMultiplier() {
     return multiplier; // difficulty;
   }

   void setDifficulty(int diff);

   int getDifficulty() {
     return difficulty;
   }

   void reset();

   bool isGameOver();

   void addToScore(int add) {
     score += add/10;
   }

   int getRawScore() {
     return score;
   }

   int getAdjustedScore();

   int getTotalNumSurvivors() {
     return totalNumSurvivors;
   }

   int getNumSaved() {
     return numSaved;
   }

   int getTotalNumZombies() {
     return totalNumZombies;
   }

private:
  Zeni::Chronometer<Zeni::Time> chrono;
   /***SINGELTON MODEL***/
  static GameModel* ptr;
  //ctor and dtor are declared private
  // create the initial objects,
	GameModel();
	// destroy all objects, 
	~GameModel();
	// no copy or assignment allowed
	GameModel(const GameModel&);
	GameModel& operator= (const GameModel&);

  Player player;
  std::tr1::shared_ptr<Zeni::Model> outpost;
  std::tr1::shared_ptr<Zeni::Model> zombie;
  std::tr1::shared_ptr<Zeni::Model> survivor;
  std::vector<SurvivalSite_sp_t> sites;

  int totalNumSurvivors;
  int numSaved;
  int totalNumZombies;
  float multiplier;
  int difficulty;
  int score;

};

//a destroyer for the singleton class Model
class GameModelDestroyer{
public:
  ~GameModelDestroyer();
};

#endif 