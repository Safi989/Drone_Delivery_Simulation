#ifndef PRIORITY_SHIPPING_STATE_H
#define PRIORITY_SHIPPING_STATE_H

#include <string>

/**
 * @brief Represents the current shipping priority of a package
 */
class PriorityShippingState {
public:
    /**
     * @brief Virtual destructor for safe deletion of derived classes
     */
    virtual ~PriorityShippingState() = default;

     /**
     * @brief Gets the name of the priority level ("Expedited", "Standard", "No Rush")
     * @return The name of the priority
     */
    virtual std::string GetName() const = 0;

    /**
     * @brief Gets the Intger value of the priority
     * Lower number means higher priority
     * @return Integer representing priority level
     */
    virtual int GetPriorityLevel() const = 0;
};

#endif