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

// ��ѯ�û������а���
Package** getUserPackages(int userId, int* count);

// ��ѯ�û��Ĵ�ȡ����
Package** getUserWaitingPackages(int userId, int* count);

// ���°���״̬
int updatePackageStatus(int packageId, int status);

// �����������
void handleMarkPackagePickedUp();

// ����ȡ����
void generatePickupCode(Package* package);

// ������������
double calculateStorageFee(Package* package);

// ��������ʷ�
double calculateShippingFee(Package* package, User* user);

// ����������ݵ��ļ�
void savePackages_File(const char* filename);

// ���ļ����ذ�������
void loadPackagesFromFile(const char* filename);

// �ͷŰ��������ڴ�

void freePackageList();


#endif /* PACKAGE_H */
