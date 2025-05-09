#include "AirplaneFactory.h"
#include "FlyingEntityDecorator.h"


IEntity* AirplaneFactory::createEntity(const JsonObject& entity) {
  std::string type = entity["type"];
  if (type.compare("airplane") == 0) {
    std::cout << "Airplane Created" << std::endl;
    Airplane* plane = new Airplane(entity);
    return new FlyingEntityDecorator(plane);
  }
  return nullptr;
}