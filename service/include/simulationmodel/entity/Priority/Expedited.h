#ifndef EXPEDITED_STATE_H
#define EXPEDITED_STATE_H

#include "PriorityShippingState.h"

class Expedited : public PriorityShippingState {
public:
    /**
     * @brief Gets shipping state.
     * @return String "Expedited"
     */
    std::string GetName() const override;

    /**
     * @brief Gets the priority level of the shipping state.
     * Lower number is higher priority.
     * @return Int representing priority level.
     */
    int GetPriorityLevel() const override;
};

#endif