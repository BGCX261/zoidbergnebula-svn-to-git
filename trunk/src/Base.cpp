#include <zenilib.h>
#include "Base.h"
#include "GameModel.h"
#include <string>

using namespace Zeni;
using std::string;

Base::Base(std::tr1::shared_ptr<Zeni::Model> outpost) : 
  SurvivalSite(outpost, Point3f(0, 0, 0)), model(outpost), position(0, 0, 0),
  purchasePoints(0), multiplier(30) {

  Vertex3f_Texture p0(Point3f(50, 50, 1), Point2f(0, 0)); 
  Vertex3f_Texture p1(Point3f(50, -50, 1), Point2f(1, 0));
  Vertex3f_Texture p2(Point3f(-50, -50, 1), Point2f(1, 1));
  Vertex3f_Texture p3(Point3f(-50, 50, 1), Point2f(0, 1));
  Material material("helipad");
  pad = Quadrilateral<Vertex3f_Texture>(p0, p1, p2, p3);
  pad.fax_Material(&material);
}

void Base::render() {
  model->set_translate(position);
  model->render();
  Video &vr = get_Video();
  vr.render(pad);
  get_Video().set_2d(
    std::pair<Point2f, Point2f>(Point2f(0,0),  get_Video().get_screen_size()), true);
  get_Video().set_ztest(false);
  get_Video().set_zwrite(false);
  Projector3D projector(GameModel::getInstance()->getPlayer().getCamera(),
    std::make_pair(Point2i(0,0), get_Video().get_screen_size()));
  Point3f point = projector.project(position);
  if (point.z > 0) {
    render_image(
        "blueBorder", // which texture to use
        Point2f(point.x - 50, point.y), // upper-left corner
        Point2f(point.x + 50, point.y + 10), // lower-right corner
        0, //theta, // rotation in radians
        1.0f, // scaling factor
        Point2f(0,0), // point to rotate & scale about
        false, // whether or not to horizontally flip the texture
        Color()); // what Color to "paint" the texture
    render_image(
        "blue", // which texture to use
        Point2f(point.x - 50, point.y), // upper-left corner
        Point2f(point.x - 50 + purchasePoints/10, point.y + 10), // lower-right corner
        0, //theta, // rotation in radians
        1.0f, // scaling factor
        Point2f(0,0), // point to rotate & scale about
        false, // whether or not to horizontally flip the texture
        Color()); // what Color to "paint" the texture

    int cost = GameModel::getInstance()->getPlayer().getSelectedSuppply().cost; 
    string name = GameModel::getInstance()->getPlayer().getSelectedSuppply().name;
    Zeni::Font &fontRender = get_Fonts()["inGame"];
    string whatColor = cost < purchasePoints ? "green" : "red";
    fontRender.render_text(itoa(cost),  Point2f(point.x, point.y + 20),
                  get_Colors()[whatColor],
                  ZENI_CENTER);
  }
  get_Video().set_3d(GameModel::getInstance()->getPlayer().getCamera());
  get_Video().set_ztest(true);
  get_Video().set_zwrite(true);
}

void Base::performLogic(float timeDelta){
  purchasePoints = purchasePoints < 1000 ?
    purchasePoints + GameModel::getInstance()->getMultiplier() * timeDelta : purchasePoints;
}

bool Base::requestSupply(enum SupplyTypes type){
  Supply request(type);
  if (request.cost < purchasePoints) {
    purchasePoints -= request.cost;
    return true;
  }
  return false;
}