#ifndef PACKAGE_H
#define PACKAGE_H

#include "main.h"

// 初始化包裹链表
void initPackageList();

// 添加包裹
Package* addPackage(int userId, int size, int weight, int note, int transportMethod,
    double value, int shelfId);

// 查找包裹（通过ID）
Package* findPackageById(int packageId);

// 查找包裹（通过取件码）
Package* findPackageByCode(const char* pickupCode);


// 查询用户的待取包裹
Package** getUserWaitingPackages(int userId);

int markPackageAsPickedUp(int packageId);

int markPackageAsAbnormal(int packageId, const char* reason);

// 处理包裹出库
void handleMarkPackagePickedUp();

// 生成取件码
void generatePickupCode(Package* package);

// 计算包裹保存费
double calculateStorageFee(Package* package);


// 保存包裹数据到文件
void savePackages_File(const char* filename);

// 从文件加载包裹数据
void loadPackagesFromFile(const char* filename);





#endif /* PACKAGE_H */
