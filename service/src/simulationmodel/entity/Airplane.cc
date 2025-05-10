#define _USE_MATH_DEFINES
#include "Airplane.h"

#include <cmath>
#include <limits>

#include "BeelineStrategy.h"


Airplane::Airplane(const JsonObject& obj) : IEntity(obj) {
  double x;
  double z;
  int num = rand() % 4;

  if (num == 0) {
    x = -1400;
    z = ((static_cast<double>(rand())) / RAND_MAX) * (1600) - 800;
  }
  else if (num == 1) {
    x = 1400;
    z = ((static_cast<double>(rand())) / RAND_MAX) * (1600) - 800;
  }
  else if (num == 2) {
    x = ((static_cast<double>(rand())) / RAND_MAX) * (2800) - 1400;
    z = -800;
  }
  else if (num == 3) {
    x = ((static_cast<double>(rand())) / RAND_MAX) * (2800) - 1400;
    z = 800;
  }

  position = Vector3(x, 700, z);
  lastPosition = position;
}

Airplane::~Airplane() {
  if (movement) delete movement;
}

void Airplane::update(double dt) {
  if (movement && !movement->isCompleted()) {
    movement->move(this, dt);

    // Calculate how far it moved since last frame
    double diff = this->lastPosition.dist(this->position);

    // Update the position for next time
    this->lastPosition = this->position;

    // Update distance traveled
    this->distanceTraveled += diff;

    // If traveled a mile
    if (this->distanceTraveled > 1625.0) {
      // Format a message and send to observers
      std::string message = this->getName() + " has traveled " +
                            std::to_string(++mileCounter) + " miles";
      this->notifyObservers(message);

      // Reset distance traveled this mile
      this->distanceTraveled = 0;
    }
  } else {
    if (movement) delete movement;

    Vector3 dest;
    dest.x = ((static_cast<double>(rand())) / RAND_MAX) * (2900) - 1400;
    dest.y = position.y;
    dest.z = ((static_cast<double>(rand())) / RAND_MAX) * (1600) - 800;
    movement = new BeelineStrategy(position, dest);
  }
}

