#ifndef STORAGE_H
#define STORAGE_H

#include "main.h"

// 初始化数据存储
void initDataStorage();

// 保存所有数据
void saveAllData();

// 加载所有数据
void loadAllData();

// 备份数据
int backupData(const char* backupPath);

// 恢复数据
int restoreData(const char* backupPath);

// 自动保存数据（定时调用）
void autoSaveData();

#endif /* STORAGE_H */