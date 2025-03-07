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



// ���»��ܵ�ǰ�������
void updateShelfCount(int shelfId, int countChange);

// ������������
float getShelfUtilization(int shelfId);

// ��ȡ���л��ܵ�ʹ�����
void getShelvesStatus(char* statusReport);

// ����������ݵ��ļ�
void saveShelvesToFile(const char* filename);

// ���ļ����ػ�������
void loadShelvesFromFile(const char* filename);




#endif /* SHELF_H */