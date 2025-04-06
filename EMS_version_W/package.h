#ifndef PACKAGE_H
#define PACKAGE_H

#include "main.h"

// ��ʼ����������
void initPackageList();

// ��Ӱ���
Package* addPackage(int userId, int size, int weight, int note, int transportMethod,
    double value, int shelfId);

// ���Ұ�����ͨ��ID��
Package* findPackageById(int packageId);

// ���Ұ�����ͨ��ȡ���룩
Package* findPackageByCode(const char* pickupCode);

void printUserPackages(Package** userPackages, int count);

// ��ѯ�û��Ĵ�ȡ����
Package** getUserWaitingPackages(int userId,int* count);

int markPackageAsPickedUp(int packageId,int choice);

int markPackageAsAbnormal(int packageId, const char* reason);

// �����������
void handleMarkPackagePickedUp();

// ����ȡ����
void generatePickupCode(Package* package);

// ������������
double calculateStorageFee(Package* package);


double doorstepfee(int size, int weight, int transportMethod);

double calculatePackageFee(int size, int weight, int transportMethod);

// ����������ݵ��ļ�
void savePackages_File(const char* filename);

// ���ļ����ذ�������
void loadPackagesFromFile(const char* filename);





#endif /* PACKAGE_H */
