#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "main.h"
#include "package.h"
#include "shelf.h"
#include "user.h"
#include "transaction.h"
#include "util.h"

// 全局包裹链表
Package* g_packageList = NULL;
// 初始化包裹链表
void initPackageList() {
g_packageList = NULL;
}

// 添加包裹
Package* addPackage(int userId, int size, int weight, int note, int transportMethod, 
            double value, int shelfId) {
// 创建新包裹
Package* newPackage = (Package*)malloc(sizeof(Package));
if (newPackage == NULL) {
return NULL;
}

// 初始化包裹数据
newPackage->id = generateUniqueId();
newPackage->userId = userId;
newPackage->size = size;
newPackage->weight = weight;
newPackage->note = note;
newPackage->transportMethod = transportMethod;
newPackage->value = value;
newPackage->shelfId = shelfId;
newPackage->status = PACKAGE_STATUS_WAITING;

// 生成取件码
generatePickupCode(newPackage);

// 获取当前时间
getCurrentTimeString(newPackage->createTime);

// 插入到链表
newPackage->next = g_packageList;
g_packageList = newPackage;

// 更新货架信息
updateShelfCount(shelfId, 1);

// 更新用户消费等级
updateUserConsumptionLevel(userId, value);

return newPackage;
}


// 生成取件码
void generatePickupCode(Package* package) {
// 格式：货架类型+日期(2位) + 货架ID(2位) + 今天的第几个包裹
char type,dateStr[3], shelfStr[3];
int dayStr;
time_t t = time(NULL);
struct tm* tm_info = localtime(&t);
sprintf(dateStr, "%02d", tm_info->tm_mday);
sprintf(shelfStr, "%02d", package->shelfId % 100);
Shelf* shelf = findShelfById(package->shelfId);
type = shelf->type + 65;
dayStr = getDailyIncrementalNumber();
sprintf(package->pickupCode, "%c%s%s%04d", type,dateStr, shelfStr, dayStr);
}

// 查找包裹（通过ID）
Package* findPackageById(int packageId) {
Package* current = g_packageList;

while (current != NULL) {
if (current->id == packageId) {
    return current;
}
current = current->next;
}

return NULL;
}

// 查找包裹（通过取件码）
Package* findPackageByCode(const char* pickupCode) {
Package* current = g_packageList;

while (current != NULL) {
if (strcmp(current->pickupCode, pickupCode) == 0) {
    return current;
}
current = current->next;
}

return NULL;
}

// 获取用户的待取包裹
Package** getUserWaitingPackages(int userId, int* count) {
// 计算待取包裹数量
*count = 0;
Package* current = g_packageList;

while (current != NULL) {
if (current->userId == userId && current->status == PACKAGE_STATUS_WAITING) {
    (*count)++;
}
current = current->next;
}

if (*count == 0) {
return NULL;
}

// 分配内存
Package** packages = (Package**)malloc(sizeof(Package*) * (*count));
if (packages == NULL) {
*count = 0;
return NULL;
}

// 填充数组
current = g_packageList;
int index = 0;

while (current != NULL && index < *count) {
if (current->userId == userId && current->status == PACKAGE_STATUS_WAITING) {
    packages[index++] = current;
}
current = current->next;
}

return packages;
}

// 标记包裹为已取出
int markPackageAsPickedUp(int packageId) {
Package* package = findPackageById(packageId);
if (package == NULL || package->status != PACKAGE_STATUS_WAITING) {
return 0;
}

package->status = PACKAGE_STATUS_PICKED;

// 更新货架信息
updateShelfCount(package->shelfId, -1);

// 如果是特殊包裹，收取保存费
if (package->note != PACKAGE_NOTE_NONE) {
double fee = calculateStorageFee(package);
add_Transaction(TRANSACTION_INCOME, INCOME_STORAGE_FEE, fee, "特殊包裹保存费");
}

return 1;
}

// 标记包裹为异常
int markPackageAsAbnormal(int packageId, const char* reason) {
Package* package = findPackageById(packageId);
if (package == NULL || package->status != PACKAGE_STATUS_WAITING) {
return 0;
}

package->status = PACKAGE_STATUS_ABNORMAL;

// 记录异常信息（待办：添加异常原因字段）

return 1;
}

// 计算包裹保存费
double calculateStorageFee(Package* package) {
// 基础保存费
double baseFee = 0.0;

// 根据包裹备注类型计算
switch (package->note) {
case PACKAGE_NOTE_FRAGILE:
    baseFee = 2.0; // 易碎品
    break;
case PACKAGE_NOTE_FRESH:
    baseFee = 5.0; // 冷鲜需要冷藏费用
    break;
default:
    return 0.0;
}

// 根据大小调整
double sizeFactor = 1.0 + (package->size * 0.2); // 大小调整因子

// 根据保存时间计算
// 这里简化处理，实际应计算从创建到现在的天数
time_t now = time(NULL);
struct tm packageTime = {0};
sscanf(package->createTime, "%d-%d-%d %d:%d:%d", 
    &packageTime.tm_year, &packageTime.tm_mon, &packageTime.tm_mday,
    &packageTime.tm_hour, &packageTime.tm_min, &packageTime.tm_sec);
packageTime.tm_year -= 1900;
packageTime.tm_mon -= 1;

time_t packageTimestamp = mktime(&packageTime);
double daysPassed = difftime(now, packageTimestamp) / (60 * 60 * 24);

// 超过3天额外收费
double timeFactor = (daysPassed > 3) ? (1.0 + (daysPassed - 3) * 0.5) : 1.0;

return baseFee * sizeFactor * timeFactor;
}

// 计算包裹费用
double calculatePackageFee(int size, int weight, int transportMethod) {
// 基础费用
double baseFee = 5.0;

// 根据大小调整
double sizeFactor = 1.0 + (size * 0.3);

// 根据重量调整
double weightFactor = 1.0 + (weight * 0.2);

// 根据运输方式调整
double transportFactor = 1.0;
switch (transportMethod) {
case TRANSPORT_NORMAL:
    transportFactor = 1.0;
    break;
case TRANSPORT_FAST_ROAD:
    transportFactor = 1.5;
    break;
case TRANSPORT_EXPRESS_AIR:
    transportFactor = 2.5;
    break;
case TRANSPORT_EXPRESS_ROAD:
    transportFactor = 2.0;
    break;
}

return baseFee * sizeFactor * weightFactor * transportFactor;
}

// 保存包裹数据到文件
void savePackages_File(const char* filename) {
FILE* file = fopen(filename, "w");
if (file == NULL) {
printf("无法打开包裹数据文件 %s\n", filename);
return;
}

Package* current = g_packageList;
while (current != NULL) {
fprintf(file, "%d,%d,%d,%d,%d,%d,%.2f,%s,%d,%d,%s\n", 
        current->id, 
        current->userId, 
        current->size, 
        current->weight, 
        current->note, 
        current->transportMethod, 
        current->value, 
        current->pickupCode, 
        current->shelfId, 
        current->status, 
        current->createTime);
current = current->next;
}

fclose(file);
}

// 从文件加载包裹数据
void loadPackagesFromFile(const char* filename) {
FILE* file = fopen(filename, "r");
if (file == NULL) {
printf("包裹数据文件 %s 不存在，将创建新文件\n", filename);
return;
}

// 清空现有链表
while (g_packageList != NULL) {
Package* temp = g_packageList;
g_packageList = g_packageList->next;
free(temp);
}

// 读取文件数据
char line[200];
while (fgets(line, sizeof(line), file)) {
int id, userId, size, weight, note, transportMethod;
double value;
char pickupCode[20], createTime[20];
int shelfId, status;

if (sscanf(line, "%d,%d,%d,%d,%d,%d,%lf,%[^,],%d,%d,%[^\n]", 
            &id, &userId, &size, &weight, &note, &transportMethod, 
            &value, pickupCode, &shelfId, &status, createTime) == 11) {
    
    // 创建新包裹节点
    Package* newPackage = (Package*)malloc(sizeof(Package));
    if (newPackage != NULL) {
        newPackage->id = id;
        newPackage->userId = userId;
        newPackage->size = size;
        newPackage->weight = weight;
        newPackage->note = note;
        newPackage->transportMethod = transportMethod;
        newPackage->value = value;
        strncpy(newPackage->pickupCode, pickupCode, sizeof(newPackage->pickupCode) - 1);
        newPackage->pickupCode[sizeof(newPackage->pickupCode) - 1] = '\0';
        newPackage->shelfId = shelfId;
        newPackage->status = status;
        strncpy(newPackage->createTime, createTime, sizeof(newPackage->createTime) - 1);
        newPackage->createTime[sizeof(newPackage->createTime) - 1] = '\0';
        
        // 插入到链表头部
        newPackage->next = g_packageList;
        g_packageList = newPackage;
        
        // 如果包裹状态为待取，更新货架计数
        if (status == PACKAGE_STATUS_WAITING) {
            updateShelfCount(shelfId, 1);
        }
    }
}
}

fclose(file);
}