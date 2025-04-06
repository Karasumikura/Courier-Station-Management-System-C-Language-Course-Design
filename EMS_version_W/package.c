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

//��������ϵͳ
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

strcpy(newPackage->abnote, "��");
newPackage->next = g_packageList;
g_packageList = newPackage;


updateShelfCount(shelfId, 1);


updateUserConsumptionLevel(userId, value);

return newPackage;
}



void generatePickupCode(Package* package) {
//ȡ��������ɹ���
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

//���ֲ��Ұ����ķ�ʽ
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
    printf("         ��ǰ�����ȡ��         \n");
    printf("=================================\n");

    int packageId;
    printf("���������ID: ");
    scanf("%d", &packageId);
    int choice;
	printf("ѡ��ȡ����ʽ��\n");
	scanf("%d", &choice);
    loading_simulation();
    if (markPackageAsPickedUp(packageId,choice)) {
        printf("�����ѳɹ����Ϊ��ȡ����\n");
        savePackages_File("packages.txt");
        saveShelvesToFile("shelves.txt");
    }
    else {
        printf("����ʧ�ܣ��������ܲ����ڻ��Ѿ�ȡ����\n");
    }

    waitForKeyPress();
}

void handleMarkPackageAbnormal() {
    clearScreen();
    printf("=================================\n");
    printf("         ��ǰ����쳣           \n");
    printf("=================================\n");

    int packageId;
    printf("���������ID: ");
    scanf("%d", &packageId);

    char reason[100];
    printf("�쳣ԭ��: ");
    scanf(" %[^\n]", reason);
    
	loading_simulation();
    if (markPackageAsAbnormal(packageId, reason)) {
        printf("�����ѳɹ����Ϊ�쳣��\n");
        savePackages_File("packages.txt");
    }
    else {
        printf("����ʧ�ܣ��������ܲ����ڻ��Ѿ����ڴ�ȡ״̬��\n");
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
add_Transaction(TRANSACTION_INCOME, INCOME_STORAGE_FEE, fee, "���������");
}
if (choice = 2) {
    double fee2 = calculateFinalPrice(packageId, doorstepfee(package->size, package->weight, package->transportMethod));
    add_Transaction(TRANSACTION_INCOME, INCOME_DOORSTEP_FEE, fee2, "��������ȡ����");
}
return 1;
}


int markPackageAsAbnormal(int packageId, char* reason) {
Package* package = findPackageById(packageId);
if (package == NULL || package->status != PACKAGE_STATUS_WAITING) {
return 0;
}
package->shelfId = -1; //�������ӻ������Ƴ�
package->status = PACKAGE_STATUS_ABNORMAL;
// ���� reason �� abnote,����������Ҫ��strcpy������ֱ�Ӹ�ֵ
strcpy(package->abnote, reason);

return 1;
}


double calculateStorageFee(Package* package) {

double baseFee = 0.0;

//�����쳣�����ķ��õķ�ʽ
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

// ��������Ϣ�������ļ���
void savePackages_File(const char* filename) {
FILE* file = fopen(filename, "w");
if (file == NULL) {
printf("�޷��򿪰��������ļ� %s\n", filename);
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

// ��ȡ�ļ��е�����
void loadPackagesFromFile(const char* filename) {
FILE* file = fopen(filename, "r");
if (file == NULL) {
printf("���������ļ� %s �����ڣ����������ļ�\n", filename);
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
        printf("û���ҵ����������İ�����\n");
		waitForKeyPress();
        return;
    }

    printf("�ҵ� %d �����������İ�����\n", count);
    for (int i = 0; i < count; i++) {
        Package* package = userPackages[i];
		char* transtypeStr;
		char* statusStr;
		switch (package->transportMethod) {
		case TRANSPORT_NORMAL:
			transtypeStr = "��ͨ��·";
			break;
		case TRANSPORT_FAST_ROAD:
			transtypeStr = "��·����";
			break;
		case TRANSPORT_EXPRESS_AIR:
			transtypeStr = "�ؿ����";
			break;
		case TRANSPORT_EXPRESS_ROAD:
			transtypeStr = "�ؿ칫·";
			break;
		default:
			transtypeStr = "δ֪";
		}
		switch (package->status) {
		case PACKAGE_STATUS_WAITING:
			statusStr = "��ȡ";
			break;
		case PACKAGE_STATUS_PICKED:
			statusStr = "��ȡ";
			break;
		case PACKAGE_STATUS_ABNORMAL:
			statusStr = "�쳣";
			break;
		default:
			statusStr = "δ֪";
		}
        printf("���� ID��%d��״̬��%s�����䷽ʽ��%s��ȡ���룺%s\n",
            package->id,statusStr,transtypeStr,package->pickupCode);
    }
    waitForKeyPress();
}