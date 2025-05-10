#ifndef ATC_H_
#define ATC_H_

#include <vector>
#include "IEntity.h"


/**
 * @brief Air Traffic Control System that manages all flying entities 
 * and makes sure they do not collide
*/
class ATC {
    public:
        /**
         * @brief Get insance of ATC
         * @return ATC instance
        */
        static ATC& getInstance();

        /**
         * @brief register a flying entity
         * @param e entity being registered
        */
        void registerEntity(IEntity* e);

        /**
         * @brief unregister a flying entity
         * @param e entity being unregistered
        */
        void unregisterEntity(IEntity* e);

        /**
         * @brief makes sure the flying entities are not colliding
        */
        void reroute();

        /**
         * @brief moves the simulation forward by a time step
         * @param dt delta time
        */
        void update(double dt);

        
    private:
        static ATC* instance;
        std::vector<IEntity*> flyingEntities;
        ATC() = default;
};

#endif
