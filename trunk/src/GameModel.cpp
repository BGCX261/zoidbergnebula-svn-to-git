#include <zenilib.h>
#include "GameModel.h"

using namespace Zeni;

static GameModelDestroyer destroyer;

GameModel* GameModel::ptr = 0;

GameModel::GameModel() : chrono(), player(),
  outpost(new Model("models/fencedOff.3ds")), zombie(new Model("models/zombie.3ds")),
    survivor(new Model("models/survivor.3ds")), totalNumSurvivors(0), numSaved(0), totalNumZombies(0), multiplier(10) {
  reset();

}

int GameModel::getAdjustedScore() {

  double returnMe =  (score);
  returnMe *= numSaved; // (numSaved + 1) / (totalNumSurvivors - numSaved);
  returnMe *= totalNumZombies;
  returnMe *= difficulty * difficulty;
  return static_cast<int>(returnMe/1000);
}

void GameModel::reset() {
  sites.clear();
  chrono.start();
  player = Player();
  sites.push_back(SurvivalSite_sp_t(new SurvivalSite(outpost, Point3f(300,300,0))));
  sites.push_back(SurvivalSite_sp_t(new SurvivalSite(outpost, Point3f(-300,200,0))));
  sites.push_back(SurvivalSite_sp_t(new SurvivalSite(outpost, Point3f(-250,-300,0))));
  sites.push_back(SurvivalSite_sp_t(new SurvivalSite(outpost, Point3f(200,-225,0))));
  sites.push_back(SurvivalSite_sp_t(new Base(outpost)));
  totalNumSurvivors = (0);
  numSaved = (0);
  totalNumZombies = (0);
  multiplier = (20);
  score = 0;
}

void GameModel::setDifficulty(int diff) {
  difficulty = diff;
  player.initValues(difficulty);
}

bool GameModel::isGameOver() {
  return sites[0]->isZombieObj() && sites[1]->isZombieObj() && sites[2]->isZombieObj() && sites[3]->isZombieObj();
}

GameModel::~GameModel() {
}

GameModel* GameModel::getInstance(){
  if(!ptr){
    ptr = new GameModel();
  }
  return ptr;
}

void GameModel::performAllLogic(float timeDelta){
  player.performLogic(timeDelta);
  player.setSite(SurvivalSite_sp_t(NULL));
  for (int i = 0; i < sites.size(); i++) {
    sites[i]->performLogic(timeDelta);
    if ( player.getBody().intersects(sites[i]->getBody())) {
      player.setSite(sites[i]);
    }
  }
}

void GameModel::renderAll(){
  for (int i = 0; i < sites.size(); i++) {
    sites[i]->render();
  }
  player.render();
}

GameModelDestroyer::~GameModelDestroyer(){
  //GameModel* model = GameModel::getInstance();
  //delete model;
}
