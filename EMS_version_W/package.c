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

// ȫ�ְ�������
Package* g_packageList = NULL;
// ��ʼ����������
void initPackageList() {
g_packageList = NULL;
}

// ��Ӱ���
Package* addPackage(int userId, int size, int weight, int note, int transportMethod, 
            double value, int shelfId) {
// �����°���
Package* newPackage = (Package*)malloc(sizeof(Package));
if (newPackage == NULL) {
return NULL;
}

// ��ʼ����������
newPackage->id = generateUniqueId();
newPackage->userId = userId;
newPackage->size = size;
newPackage->weight = weight;
newPackage->note = note;
newPackage->transportMethod = transportMethod;
newPackage->value = value;
newPackage->shelfId = shelfId;
newPackage->status = PACKAGE_STATUS_WAITING;

// ����ȡ����
generatePickupCode(newPackage);

// ��ȡ��ǰʱ��
getCurrentTimeString(newPackage->createTime);

// ���뵽����
newPackage->next = g_packageList;
g_packageList = newPackage;

// ���»�����Ϣ
updateShelfCount(shelfId, 1);

// �����û����ѵȼ�
updateUserConsumptionLevel(userId, value);

return newPackage;
}


// ����ȡ����
void generatePickupCode(Package* package) {
// ��ʽ����������+����(2λ) + ����ID(2λ) + ����ĵڼ�������
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

// ���Ұ�����ͨ��ID��
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

// ���Ұ�����ͨ��ȡ���룩
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

// ��ȡ�û��Ĵ�ȡ����
Package** getUserWaitingPackages(int userId, int* count) {
// �����ȡ��������
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

// �����ڴ�
Package** packages = (Package**)malloc(sizeof(Package*) * (*count));
if (packages == NULL) {
*count = 0;
return NULL;
}

// �������
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

// ��ǰ���Ϊ��ȡ��
int markPackageAsPickedUp(int packageId) {
Package* package = findPackageById(packageId);
if (package == NULL || package->status != PACKAGE_STATUS_WAITING) {
return 0;
}

package->status = PACKAGE_STATUS_PICKED;

// ���»�����Ϣ
updateShelfCount(package->shelfId, -1);

// ����������������ȡ�����
if (package->note != PACKAGE_NOTE_NONE) {
double fee = calculateStorageFee(package);
add_Transaction(TRANSACTION_INCOME, INCOME_STORAGE_FEE, fee, "������������");
}

return 1;
}

// ��ǰ���Ϊ�쳣
int markPackageAsAbnormal(int packageId, const char* reason) {
Package* package = findPackageById(packageId);
if (package == NULL || package->status != PACKAGE_STATUS_WAITING) {
return 0;
}

package->status = PACKAGE_STATUS_ABNORMAL;

// ��¼�쳣��Ϣ�����죺����쳣ԭ���ֶΣ�

return 1;
}

// ������������
double calculateStorageFee(Package* package) {
// ���������
double baseFee = 0.0;

// ���ݰ�����ע���ͼ���
switch (package->note) {
case PACKAGE_NOTE_FRAGILE:
    baseFee = 2.0; // ����Ʒ
    break;
case PACKAGE_NOTE_FRESH:
    baseFee = 5.0; // ������Ҫ��ط���
    break;
default:
    return 0.0;
}

// ���ݴ�С����
double sizeFactor = 1.0 + (package->size * 0.2); // ��С��������

// ���ݱ���ʱ�����
// ����򻯴���ʵ��Ӧ����Ӵ��������ڵ�����
time_t now = time(NULL);
struct tm packageTime = {0};
sscanf(package->createTime, "%d-%d-%d %d:%d:%d", 
    &packageTime.tm_year, &packageTime.tm_mon, &packageTime.tm_mday,
    &packageTime.tm_hour, &packageTime.tm_min, &packageTime.tm_sec);
packageTime.tm_year -= 1900;
packageTime.tm_mon -= 1;

time_t packageTimestamp = mktime(&packageTime);
double daysPassed = difftime(now, packageTimestamp) / (60 * 60 * 24);

// ����3������շ�
double timeFactor = (daysPassed > 3) ? (1.0 + (daysPassed - 3) * 0.5) : 1.0;

return baseFee * sizeFactor * timeFactor;
}

// �����������
double calculatePackageFee(int size, int weight, int transportMethod) {
// ��������
double baseFee = 5.0;

// ���ݴ�С����
double sizeFactor = 1.0 + (size * 0.3);

// ������������
double weightFactor = 1.0 + (weight * 0.2);

// �������䷽ʽ����
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

// ����������ݵ��ļ�
void savePackages_File(const char* filename) {
FILE* file = fopen(filename, "w");
if (file == NULL) {
printf("�޷��򿪰��������ļ� %s\n", filename);
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

// ���ļ����ذ�������
void loadPackagesFromFile(const char* filename) {
FILE* file = fopen(filename, "r");
if (file == NULL) {
printf("���������ļ� %s �����ڣ����������ļ�\n", filename);
return;
}

// �����������
while (g_packageList != NULL) {
Package* temp = g_packageList;
g_packageList = g_packageList->next;
free(temp);
}

// ��ȡ�ļ�����
char line[200];
while (fgets(line, sizeof(line), file)) {
int id, userId, size, weight, note, transportMethod;
double value;
char pickupCode[20], createTime[20];
int shelfId, status;

if (sscanf(line, "%d,%d,%d,%d,%d,%d,%lf,%[^,],%d,%d,%[^\n]", 
            &id, &userId, &size, &weight, &note, &transportMethod, 
            &value, pickupCode, &shelfId, &status, createTime) == 11) {
    
    // �����°����ڵ�
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
        
        // ���뵽����ͷ��
        newPackage->next = g_packageList;
        g_packageList = newPackage;
        
        // �������״̬Ϊ��ȡ�����»��ܼ���
        if (status == PACKAGE_STATUS_WAITING) {
            updateShelfCount(shelfId, 1);
        }
    }
}
}

fclose(file);
}