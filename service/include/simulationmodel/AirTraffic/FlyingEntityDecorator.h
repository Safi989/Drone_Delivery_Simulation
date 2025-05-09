#ifndef FlyingEntityDecorator_H_
#define FlyingEntityDecorator_H_

#include "IEntityDecorator.h"
#include "ATC.h"


class FlyingEntityDecorator : public IEntityDecorator<IEntity> {
    public:
        FlyingEntityDecorator(IEntity* e) : IEntityDecorator<IEntity>(e) {
            ATC::getInstance().registerEntity(this);
        }

        ~FlyingEntityDecorator() override {
            ATC::getInstance().unregisterEntity(this);
        }

        void update(double dt) override {
            IEntityDecorator<IEntity>::update(dt);
        }
};

#endif