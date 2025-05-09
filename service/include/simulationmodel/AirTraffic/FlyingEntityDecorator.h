#ifndef FlyingEntityDecorator_H_
#define FlyingEntityDecorator_H_

#include "IEntityDecorator.h"
#include "ATC.h"

/**
 * @brief wraps the IEntiity to make it aware of the ATC
*/
class FlyingEntityDecorator : public IEntityDecorator<IEntity> {
    public:
        /**
         * @brief makes a new FlyingEntityDecorator and registers to ATC
         * @param e pointer to the entity to wrap
        */
        FlyingEntityDecorator(IEntity* e) : IEntityDecorator<IEntity>(e) {
            ATC::getInstance().registerEntity(this);
        }

        /**
         * @brief destroy the FlyingEntityDecorator and unregister from ATC
        */
        ~FlyingEntityDecorator() override {
            ATC::getInstance().unregisterEntity(this);
        }

        /**
         * @brief update the entity
         * @param dt delta time 
        */
        void update(double dt) override {
            IEntityDecorator<IEntity>::update(dt);
        }
};

#endif