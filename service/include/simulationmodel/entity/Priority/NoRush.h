#ifndef NORUSH_STATE_H
#define NORUSH_STATE_H

#include "PriorityShippingState.h"
/**
 * @brief Represents a lowest priority shipping state
 */
class NoRush : public PriorityShippingState {
public:
    /**
     * @brief Gets the name of this priority state
     * @return "NoRush"
     */

    std::string GetName() const override;

    /**
     * @brief Gets the priority value for sorting
     * Higher value means lower priority
     * @return Integer representing NoRush priority
     */
    int GetPriorityLevel() const override;
};

#endif