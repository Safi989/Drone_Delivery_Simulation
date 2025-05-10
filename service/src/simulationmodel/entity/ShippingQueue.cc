#include "ShippingQueue.h"
#include <algorithm>

void ShippingQueue::AddPackage(Package* package) {
    queue.push_back(package);
    SortQueue();
}

void ShippingQueue::UpdatePackagePriority(Package* package) {
    int idxToRemove = -1;

    for (size_t i = 0; i < queue.size(); ++i) {
        if (queue[i] == package) {
            idxToRemove = static_cast<int>(i);
            break;
        }
    }
    if (idxToRemove != -1) {

        queue.erase(queue.begin() + idxToRemove);
        queue.push_back(package);
        SortQueue();
    }
}

Package* ShippingQueue::GetNextPackage() {
    for (Package* p : queue) {
        if (!p->isPickedUp() && !p->isDelivered()) {
            return p;
        }
    }
    return nullptr; 
}

std::vector<Package*> ShippingQueue::GetUndeliveredPackages() const {
    return queue;
}


void ShippingQueue::RemoveDeliveredPackages() {
    for (size_t i = 0; i < queue.size(); ) {
        if (queue[i]->isDelivered()) {
            queue.erase(queue.begin() + i);

        } else {
            ++i;
        }
    }
}

void ShippingQueue::SortQueue() {
    std::stable_sort(queue.begin(), queue.end(), [](Package* a, Package* b) {
        return a->GetPriorityLevel() < b->GetPriorityLevel(); 
    });
}