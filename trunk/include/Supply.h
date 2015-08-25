#ifndef SUPPLY_H
#define SUPPLY_H


enum SupplyTypes {BAT, PISTOL, RIFLE, RESCUE, SURVIVOR};

struct Supply {
  Supply(SupplyTypes type_) : type(type_) {
    switch (type) {
      case BAT:
        value = 1;
        soundName = "cricketBat";
        imgName = "cricketBat";
        name = "Cricket Bats";
        cost = 100;
        break;
      case PISTOL:
        value = 4;
        soundName = "pistol";
        imgName = "pistol";
        name = "Pistols";
        cost = 300;
        break;
      case RIFLE:
        value = 8;
        soundName = "sayHello";
        imgName = "rifle";
        name = "Machine Gun";
        cost = 600;
        break;
      case RESCUE:
        value = 4;
        soundName = "rescue";
        imgName = "lifeRing";
        name = "Life Ring";
        cost = 900;
        break;
      case SURVIVOR:
        break;
      default:
        value = 1;
        soundName = "cricketBat";
    }
  }
  SupplyTypes type;
  int value;
  int cost;
  std::string soundName;
  std::string imgName;
  std::string name;
};


#endif
