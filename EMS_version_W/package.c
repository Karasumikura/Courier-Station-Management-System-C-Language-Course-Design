#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "main.h"
#include "package.h"
#include "shelf.h"
#include "user.h"
#include "pricing.h"
#include "transaction.h"
#include "util.h"

//包裹管理系统
Package* g_packageList = NULL;

void initPackageList() {
g_packageList = NULL;
}


Package* addPackage(int userId, int size, int weight, int note, int transportMethod, 
            double value, int shelfId) {

Package* newPackage = (Package*)malloc(sizeof(Package));
if (newPackage == NULL) {
return NULL;
}


newPackage->id = generateUniqueId();
newPackage->userId = userId;
newPackage->size = size;
newPackage->weight = weight;
newPackage->note = note;
newPackage->transportMethod = transportMethod;
newPackage->value = value;
newPackage->shelfId = shelfId;
newPackage->status = PACKAGE_STATUS_WAITING;


generatePickupCode(newPackage);


getCurrentTimeString(newPackage->createTime);

strcpy(newPackage->abnote, "无");
newPackage->next = g_packageList;
g_packageList = newPackage;


updateShelfCount(shelfId, 1);


updateUserConsumptionLevel(userId, value);

return newPackage;
}



void generatePickupCode(Package* package) {
//取件码的生成规则
char type,dateStr[3];
int dayStr;
time_t t = time(NULL);
struct tm* tm_info = localtime(&t);
sprintf(dateStr, "%02d", tm_info->tm_mday);
srand((unsigned int)time(NULL));
int random = rand() % 90 + 10;
Shelf* shelf = findShelfById(package->shelfId);
type = shelf->type + 65;
dayStr = getDailyIncrementalNumber();
sprintf(package->pickupCode, "%c%s%d%04d", type,dateStr, random, dayStr);
}

//两种查找包裹的方式
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

void handleMarkPackagePickedUp() {
    clearScreen();
    printf("=================================\n");
    printf("         标记包裹已取出         \n");
    printf("=================================\n");

    int packageId;
    printf("请输入包裹ID: ");
    scanf("%d", &packageId);
    int choice;
	printf("选择取件方式：\n");
	scanf("%d", &choice);
    loading_simulation();
    if (markPackageAsPickedUp(packageId,choice)) {
        printf("包裹已成功标记为已取出！\n");
        savePackages_File("packages.txt");
        saveShelvesToFile("shelves.txt");
    }
    else {
        printf("操作失败，包裹可能不存在或已经取出！\n");
    }

    waitForKeyPress();
}

void handleMarkPackageAbnormal() {
    clearScreen();
    printf("=================================\n");
    printf("         标记包裹异常           \n");
    printf("=================================\n");

    int packageId;
    printf("请输入包裹ID: ");
    scanf("%d", &packageId);

    char reason[100];
    printf("异常原因: ");
    scanf(" %[^\n]", reason);
    
	loading_simulation();
    if (markPackageAsAbnormal(packageId, reason)) {
        printf("包裹已成功标记为异常！\n");
        savePackages_File("packages.txt");
    }
    else {
        printf("操作失败，包裹可能不存在或已经不在待取状态！\n");
    }

    waitForKeyPress();
}

Package** getUserWaitingPackages(int userId,int *count) {
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

Package** packages = (Package**)malloc(sizeof(Package*) * (*count));
if (packages == NULL) {
*count = 0;
return NULL;
}

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

int markPackageAsPickedUp(int packageId,int choice) {
Package* package = findPackageById(packageId);
if (package == NULL || package->status != PACKAGE_STATUS_WAITING) {
return 0;
}

package->status = PACKAGE_STATUS_PICKED;

updateShelfCount(package->shelfId, -1);

if (package->note != PACKAGE_NOTE_NONE) {
double fee = calculateFinalPrice(packageId,calculateStorageFee(package));
add_Transaction(TRANSACTION_INCOME, INCOME_STORAGE_FEE, fee, "包裹保存费");
}
if (choice = 2) {
    double fee2 = calculateFinalPrice(packageId, doorstepfee(package->size, package->weight, package->transportMethod));
    add_Transaction(TRANSACTION_INCOME, INCOME_DOORSTEP_FEE, fee2, "包裹上门取件费");
}
return 1;
}


int markPackageAsAbnormal(int packageId, char* reason) {
Package* package = findPackageById(packageId);
if (package == NULL || package->status != PACKAGE_STATUS_WAITING) {
return 0;
}
package->shelfId = -1; //将包裹从货架上移除
package->status = PACKAGE_STATUS_ABNORMAL;
// 复制 reason 到 abnote,这里数组需要用strcpy而不是直接赋值
strcpy(package->abnote, reason);

return 1;
}


double calculateStorageFee(Package* package) {

double baseFee = 0.0;

//计算异常包裹的费用的方式
switch (package->note) {
case PACKAGE_NOTE_FRAGILE:
    baseFee = 2.0; 
    break;
case PACKAGE_NOTE_FRESH:
    baseFee = 1.5; 
    break;
default:
    return 0.0;
}


double sizeFactor = 1.0 + (package->size * 0.2); 


time_t now = time(NULL);
struct tm packageTime = {0};
sscanf(package->createTime, "%d-%d-%d %d:%d:%d", 
    &packageTime.tm_year, &packageTime.tm_mon, &packageTime.tm_mday,
    &packageTime.tm_hour, &packageTime.tm_min, &packageTime.tm_sec);
packageTime.tm_year -= 1900;
packageTime.tm_mon -= 1;

time_t packageTimestamp = mktime(&packageTime);
double daysPassed = difftime(now, packageTimestamp) / (60 * 60 * 24);


double timeFactor = (daysPassed > 3) ? (1.0 + (daysPassed - 3) * 0.5) : 1.0;

return baseFee * sizeFactor * timeFactor;
}

double calculatePackageFee(int size, int weight, int transportMethod) {
    
    double baseFee = 1.0;

    
    double sizeFactor = 1.0 + (size * 0.3);

   
    double weightFactor = 1.0 + (weight * 0.2);

    
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

double doorstepfee(int size, int weight, int transportMethod) {
    
    double baseFee = 1.0;

    
    double sizeFactor = 1.0 + (size * 0.2);

    
    double weightFactor = 1.0 + (weight * 0.1);

    
    double transportFactor = 1.0;
    switch (transportMethod) {
    case TRANSPORT_NORMAL:
        transportFactor = 1.0;
        break;
    case TRANSPORT_FAST_ROAD:
        transportFactor = 1.2;
        break;
    case TRANSPORT_EXPRESS_AIR:
        transportFactor = 1.5;
        break;
    case TRANSPORT_EXPRESS_ROAD:
        transportFactor = 1.4;
        break;
    }

    return baseFee * sizeFactor * weightFactor * transportFactor;
}

// 将包裹信息导出到文件中
void savePackages_File(const char* filename) {
FILE* file = fopen(filename, "w");
if (file == NULL) {
printf("无法打开包裹数据文件 %s\n", filename);
return;
}

Package* current = g_packageList;
while (current != NULL) {
fprintf(file, "%d,%d,%d,%d,%d,%d,%.2f,%s,%d,%d,%s,%s\n", 
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
        current->createTime,
        current->abnote);
current = current->next;
}

fclose(file);
}

// 读取文件中的数据
void loadPackagesFromFile(const char* filename) {
FILE* file = fopen(filename, "r");
if (file == NULL) {
printf("包裹数据文件 %s 不存在，将创建新文件\n", filename);
return;
}


while (g_packageList != NULL) {
Package* temp = g_packageList;
g_packageList = g_packageList->next;
free(temp);
}


char line[200];
while (fgets(line, sizeof(line), file)) {
int id, userId, size, weight, note, transportMethod;
double value;
char pickupCode[20], createTime[20],abnote[50];
int shelfId, status;

if (sscanf(line, "%d,%d,%d,%d,%d,%d,%lf,%[^,],%d,%d,%[^,],%[^\n]", 
            &id, &userId, &size, &weight, &note, &transportMethod, 
            &value, pickupCode, &shelfId, &status, createTime, abnote) == 12) {
    
    
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
        strncpy(newPackage->abnote, abnote, sizeof(newPackage->abnote) - 1);
        newPackage->abnote[sizeof(newPackage->abnote) - 1] = '\0';
        
        
        newPackage->next = g_packageList;
        g_packageList = newPackage;
        
        
        if (status == PACKAGE_STATUS_WAITING) {
            updateShelfCount(shelfId, 1);
        }
    }
}
}

fclose(file);
}

void printUserPackages(Package** userPackages, int count) {
    if (userPackages == NULL || count == 0) {
        printf("没有找到符合条件的包裹。\n");
		waitForKeyPress();
        return;
    }

    printf("找到 %d 个符合条件的包裹：\n", count);
    for (int i = 0; i < count; i++) {
        Package* package = userPackages[i];
		char* transtypeStr;
		char* statusStr;
		switch (package->transportMethod) {
		case TRANSPORT_NORMAL:
			transtypeStr = "普通公路";
			break;
		case TRANSPORT_FAST_ROAD:
			transtypeStr = "公路速运";
			break;
		case TRANSPORT_EXPRESS_AIR:
			transtypeStr = "特快空运";
			break;
		case TRANSPORT_EXPRESS_ROAD:
			transtypeStr = "特快公路";
			break;
		default:
			transtypeStr = "未知";
		}
		switch (package->status) {
		case PACKAGE_STATUS_WAITING:
			statusStr = "待取";
			break;
		case PACKAGE_STATUS_PICKED:
			statusStr = "已取";
			break;
		case PACKAGE_STATUS_ABNORMAL:
			statusStr = "异常";
			break;
		default:
			statusStr = "未知";
		}
        printf("包裹 ID：%d，状态：%s，运输方式：%s，取件码：%s\n",
            package->id,statusStr,transtypeStr,package->pickupCode);
    }
    waitForKeyPress();
}