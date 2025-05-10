#ifndef STANDARD_STATE_H
#define STANDARD_STATE_H

#include "PriorityShippingState.h"

/**
 * @brief Represents the default shipping priority level
 */
class Standard : public PriorityShippingState {
public:
     /**
     * @brief Gets the name of this priority state
     * @return "Standard"
     */
    std::string GetName() const override;

     /**
     * @brief Gets the priority value for sorting
     * @return Integer representing Standard priority
     */
    int GetPriorityLevel() const override;
};

#endif