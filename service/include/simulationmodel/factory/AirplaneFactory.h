#ifndef Airplane_FACTORY_H_
#define Airplane_FACTORY_H_

#include <vector>

#include "Airplane.h"
#include "IEntityFactory.h"

/**
 *@brief Airplane Factory to produce Helicopter class.
 **/
class AirplaneFactory : public IEntityFactory {
 public:
  /**
   * @brief Destructor for AirplaneFactory class.
   **/
  virtual ~AirplaneFactory() {}

  /**
   * @brief Creates entity using the given JSON object, if possible.
   * @param entity - JsonObject to be used to create the new entity.
   * @return Entity that was created if it was created successfully, or a
   *nullpointer if creation failed.
   **/
  IEntity* createEntity(const JsonObject& entity);
};

#endif
