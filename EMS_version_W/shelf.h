#ifndef SHELF_H
#define SHELF_H

#include "main.h"

// ��ʼ����������
void initShelfList();

// ��ӻ���
Shelf* addShelf(int type, int capacity);

// ���һ��ܣ�ͨ��ID��
Shelf* findShelfById(int shelfId);

// ���Һ��ʵĻ��ܷ��ð���
int findSuitableShelf(int packageSize, int packageNote);

// ���»�����Ϣ
int updateShelf(int shelfId, int capacity);

// ���»��ܵ�ǰ�������
void updateShelfCount(int shelfId, int countChange);

// �������Ƿ�����
int isShelfFull(int shelfId);

// ����������ݵ��ļ�
void saveShelvesToFile(const char* filename);

// ���ļ����ػ�������
void loadShelvesFromFile(const char* filename);

// �ͷŻ��������ڴ�
void freeShelfList();

#endif /* SHELF_H */