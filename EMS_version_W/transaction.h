#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "main.h"

// 初始化交易记录链表
void initTransactionList();

// 添加交易记录
Transaction* add_Transaction(int type, int subType, double amount, const char* description);

// 查找交易记录（通过ID）
Transaction* findTransactionById(int transactionId);

// 查询时间范围内的交易记录
Transaction** getTransactionsByDateRange(const char* startDate, const char* endDate, int* count);

// 计算指定日期范围内的收入总额
double calculateTotalIncome(const char* startDate, const char* endDate);

// 计算指定日期范围内的支出总额
double calculateTotalExpense(const char* startDate, const char* endDate);

// 保存交易记录到文件
void saveTransactionsToFile(const char* filename);

// 从文件加载交易记录
void loadTransactionsFromFile(const char* filename);

// 释放交易记录链表内存
void freeTransactionList();


#endif /* TRANSACTION_H */