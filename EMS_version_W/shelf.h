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

// 更新货架信息
int updateShelf(int shelfId, int capacity);

// 更新货架当前存放数量
void updateShelfCount(int shelfId, int countChange);

// 检查货架是否满载
int isShelfFull(int shelfId);

// 保存货架数据到文件
void saveShelvesToFile(const char* filename);

// 从文件加载货架数据
void loadShelvesFromFile(const char* filename);

// 释放货架链表内存
void freeShelfList();

#endif /* SHELF_H */