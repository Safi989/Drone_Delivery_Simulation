#include "HelicopterFactory.h"
#include "FlyingEntityDecorator.h"


IEntity* HelicopterFactory::createEntity(const JsonObject& entity) {
  std::string type = entity["type"];
  if (type.compare("helicopter") == 0) {
    std::cout << "Helicopter Created" << std::endl;
    Helicopter* heli = new Helicopter(entity);
    return new FlyingEntityDecorator(heli);
  }
  return nullptr;
}
