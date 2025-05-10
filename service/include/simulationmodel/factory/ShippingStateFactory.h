#ifndef SHIPPING_STATE_FACTORY_H
#define SHIPPING_STATE_FACTORY_H

#include <string>
#include "PriorityShippingState.h"

/**
 * @brief Creates the correct shipping state (NoRush, Standard, Expedited)
 */
class ShippingStateFactory {
public:
    /**
     * @brief Returns a shipping priority state based on a string
     * @param priority The name of the shipping priority ("Expedited", "Standard", "NoRush")
     * @return A new PriorityShippingState pointer
     */
    static PriorityShippingState* CreatePriority(const std::string& priority);
};

#endif