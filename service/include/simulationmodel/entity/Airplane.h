#ifndef Airplane_H_
#define Airplane_H_

#include "IEntity.h"
#include "IStrategy.h"

class Airplane : public IEntity {
 public:
  /**
   * @brief Airplanes are created with a name
   * @param obj JSON object containing the drone's information
   */
  Airplane(const JsonObject& obj);

  ~Airplane();

  void update(double dt);


 private:
  IStrategy* movement = nullptr;
  double distanceTraveled = 0;
  unsigned int mileCounter = 0;
  Vector3 lastPosition;
};

#endif

