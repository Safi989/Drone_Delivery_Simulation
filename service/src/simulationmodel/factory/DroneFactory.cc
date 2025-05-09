#include "DroneFactory.h"
#include "FlyingEntityDecorator.h"


IEntity* DroneFactory::createEntity(const JsonObject& entity) {
  std::string type = entity["type"];
  if (type.compare("drone") == 0) {
    std::cout << "Drone Created" << std::endl;
    Drone* drone = new DroneColorDecorator(new Drone(entity), 0, 0, 100);
    return new FlyingEntityDecorator(drone);
  }
  return nullptr;
}
