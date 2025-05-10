#include "Package.h"

#include "Robot.h"

#include "Expedited.h"
#include "Standard.h"
#include "NoRush.h"

Package::Package(const JsonObject& obj) : IEntity(obj) {}

Vector3 Package::getDestination() const { return destination; }

std::string Package::getStrategyName() const { return strategyName; }

Robot* Package::getOwner() const { return owner; }

bool Package::requiresDelivery() const { return requiresDelivery_; }

void Package::setStrategyName(std::string strategyName_) {
  strategyName = strategyName_;
}

void Package::update(double dt) {}

void Package::initDelivery(Robot* owner) {
  this->owner = owner;
  owner->requestedDelivery = false;
  requiresDelivery_ = false;
  destination = owner->getPosition();
}

void Package::handOff() {
  if (owner) {
    owner->receive(this);
  }
}

void Package::setPriority(PriorityShippingState* newState) {
  if (!pickedUp && !delivered) {
    this->priorityState = newState;

    notifyObservers("Package " + getName() + " priority changed to " + newState->GetName());
  }
}

std::string Package::getPriorityName() const {
  if (priorityState) {
    return priorityState->GetName();
  } else {
      return "not assigned";
  }
}

int Package::GetPriorityLevel() const {
  if (priorityState) {
    return priorityState->GetPriorityLevel();
  } else {
      return 4;
  }
}

void Package::markPickedUp() {
  pickedUp = true;
  notifyObservers("Package picked up");
}

void Package::markDelivered() {
  delivered = true;
  notifyObservers("Package delivered");
}

bool Package::isPickedUp() const {
  return pickedUp;
}

bool Package::isDelivered() const {
  return delivered;
}
