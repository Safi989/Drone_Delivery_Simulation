#ifndef SHIPPING_QUEUE_H
#define SHIPPING_QUEUE_H

#include <vector>
#include <algorithm>
#include "Package.h" 
/*
 * @brief Manages packages based on their shipping priority
 */
class ShippingQueue {
public:
    /**
     * @brief Adds a package to the queue
     * @param package Pointer to the package
     */
    void AddPackage(Package* package);

    /**
     * @brief Updates a package's position in the queue when its priority changes
     * @param package Pointer to the package
     */
    void UpdatePackagePriority(Package* package);

    /**
     * @brief Gets the next package to be delivered
     * @return Pointer to the highest priority package
     */
    Package* GetNextPackage();

    /**
     * @brief Gets all undelivered packages in the queue
     * @return Vector of the undelivered package pointers
     */
    std::vector<Package*> GetUndeliveredPackages() const;

    /**
     * @brief Removes packages that have already been delivered
     */
    void RemoveDeliveredPackages();

private:
    std::vector<Package*> queue;
    /**
     * @brief Sorts the queue based on package priority
     */
    void SortQueue();
};

#endif