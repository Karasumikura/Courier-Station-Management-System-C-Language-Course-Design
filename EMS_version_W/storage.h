#ifndef STORAGE_H
#define STORAGE_H

#include "main.h"

// ��ʼ�����ݴ洢
void initDataStorage();

// ������������
void saveAllData();

// ������������
void loadAllData();

// ��������
int backupData(const char* backupPath);

// �ָ�����
int restoreData(const char* backupPath);

// �Զ��������ݣ���ʱ���ã�
void autoSaveData();

#endif /* STORAGE_H */