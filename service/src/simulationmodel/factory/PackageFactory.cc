#include "PackageFactory.h"
#include "ShippingQueue.h"
#include "ShippingStateFactory.h"


IEntity* PackageFactory::createEntity(const JsonObject& entity) {

  std::string type = entity["type"];
  if (type == "package") {
    Package* p = new Package(entity);

    std::string priority = "Standard";
    if (entity.contains("priority")) {
      priority = static_cast<std::string>(entity["priority"]);
    }

    p->setPriority(ShippingStateFactory::CreatePriority(priority));
    shippingQueue->AddPackage(p);


    std::cout << "Package Made" << std::endl;

    return p;
  }
  return nullptr;
}

