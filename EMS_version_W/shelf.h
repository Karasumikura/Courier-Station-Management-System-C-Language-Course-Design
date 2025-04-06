#ifndef SHELF_H
#define SHELF_H

#include "main.h"

// 货架数据系统
void initShelfList();


Shelf* addShelf(int type, int capacity);


Shelf* findShelfById(int shelfId);


int findSuitableShelf(int packageSize, int packageNote);



void updateShelfCount(int shelfId, int countChange);

float getShelfUtilization(int shelfId);


void getShelvesStatus(char* statusReport);

void saveShelvesToFile(const char* filename);

void loadShelvesFromFile(const char* filename);




#endif /* SHELF_H */