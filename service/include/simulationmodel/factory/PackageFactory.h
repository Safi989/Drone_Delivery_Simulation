#ifndef PACKAGE_FACTORY_H_
#define PACKAGE_FACTORY_H_

#include <vector>

#include "IEntityFactory.h"
#include "Package.h"

#include "Expedited.h"
#include "Standard.h"
#include "NoRush.h"
#include "ShippingQueue.h"

class ShippingQueue;

/**
 *@brief Package Factory to produce Package class.
 **/
class PackageFactory : public IEntityFactory {
 public:

  PackageFactory(ShippingQueue* queue) : shippingQueue(queue) {}

  /**
   * @brief Destructor for PackageFactory class.
   **/
  virtual ~PackageFactory() {}

  /**
   * @brief Creates entity using the given JSON object, if possible.
   * @param entity - JsonObject to be used to create the new entity.
   * @return Entity that was created if it was created successfully, or a
   *nullpointer if creation failed.
   **/
  IEntity* createEntity(const JsonObject& entity);

  private:
    ShippingQueue* shippingQueue;
};

#endif
