#include "ATC.h"
#include <algorithm>
#include <cmath>
#include <iostream>

ATC* ATC::instance = nullptr;

ATC& ATC::getInstance() {
    if (instance == nullptr) {
        instance = new ATC();
    }
    return *instance;
}


void ATC::registerEntity(IEntity* e) {
    flyingEntities.push_back(e);
}


void ATC::unregisterEntity(IEntity* e) {
    flyingEntities.erase(std::remove(flyingEntities.begin(), flyingEntities.end(), e), flyingEntities.end());
}


void ATC::enforceSeparation() {
    const double minDist = 50.0;

    for (size_t i = 0; i < flyingEntities.size(); i++) {
        for (size_t j =  i + 1; j < flyingEntities.size(); j++) {
            Vector3 a = flyingEntities[i]->getPosition();
            Vector3 b = flyingEntities[j]->getPosition();

            double dx = a.x - b.x;
            double dy = a.y - b.y;
            double dz = a.z - b.z;
            double squaredDist = (dx * dx) + (dy * dy) + (dz * dz);

            if (squaredDist < (minDist * minDist)) {
                double dist = std::sqrt(squaredDist);
                double in = minDist - dist;

                Vector3 norm;
                norm.x = dx / dist;
                norm.y = dy / dist;
                norm.z = dz / dist;

                double push = in * 0.5;
                Vector3 offset{norm.x * push, norm.y * push, norm.z * push};

                Vector3 newA{a.x + offset.x, a.y + offset.y, a.z + offset.z};
                Vector3 newB{b.x - offset.x, b.y - offset.y, b.z - offset.z};

                flyingEntities[i]->setPosition(newA);
                flyingEntities[j]->setPosition(newB);
            }
        }
    }
}



void ATC::update(double dt) {
    for (std::size_t i = 0; i < flyingEntities.size(); i++) {
        flyingEntities[i]->update(dt);
    }
    enforceSeparation();
}