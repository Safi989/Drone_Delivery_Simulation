#include "ShippingStateFactory.h"
#include "Expedited.h"
#include "Standard.h"
#include "NoRush.h"

PriorityShippingState* ShippingStateFactory::CreatePriority(const std::string& priority) {
    if (priority == "Expedited") {
        return new Expedited();
    } else if (priority == "NoRush") {
        return new NoRush();
    } else {
        return new Standard();
    }
}