#ifndef SHELF_H
#define SHELF_H

#include "main.h"

// 初始化货架链表
void initShelfList();

// 添加货架
Shelf* addShelf(int type, int capacity);

// 查找货架（通过ID）
Shelf* findShelfById(int shelfId);

// 查找合适的货架放置包裹
int findSuitableShelf(int packageSize, int packageNote);



// 更新货架当前存放数量
void updateShelfCount(int shelfId, int countChange);

// 检查货架利用率
float getShelfUtilization(int shelfId);

// 获取所有货架的使用情况
void getShelvesStatus(char* statusReport);

// 保存货架数据到文件
void saveShelvesToFile(const char* filename);

// 从文件加载货架数据
void loadShelvesFromFile(const char* filename);




#endif /* SHELF_H */