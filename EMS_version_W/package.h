#ifndef PACKAGE_H
#define PACKAGE_H

#include "main.h"

// °ü¹üÏµÍ³
void initPackageList();

 
Package* addPackage(int userId, int size, int weight, int note, int transportMethod,
    double value, int shelfId);


Package* findPackageById(int packageId);


Package* findPackageByCode(const char* pickupCode);

void printUserPackages(Package** userPackages, int count);


Package** getUserWaitingPackages(int userId,int* count);

double markPackageAsPickedUp(int packageId,int choice);

Package** getUserAbnormalPackages(int userId, int* count);

void printUserAbnormalPackages(Package** userPackages, int count);

int markPackageAsAbnormal(int packageId, const char* reason);

void handleMarkPackagePickedUp();


void generatePickupCode(Package* package);


double calculateStorageFee(Package* package);


double doorstepfee(int size, int weight, int transportMethod);

double calculatePackageFee(int size, int weight, int transportMethod);


void savePackages_File(const char* filename);


void loadPackagesFromFile(const char* filename);





#endif /* PACKAGE_H */
