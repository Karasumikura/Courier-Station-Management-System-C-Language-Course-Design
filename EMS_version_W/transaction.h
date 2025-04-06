#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "main.h"


void initTransactionList();


Transaction* add_Transaction(int type, int subType, double amount, const char* description);

Transaction** getTransactionsByDateRange(const char* startDate, const char* endDate, int* count);

// 收入
double calculateTotalIncome(const char* startDate, const char* endDate);

// 支出
double calculateTotalExpense(const char* startDate, const char* endDate);

// 将数据导出至文件中
void saveTransactionsToFile(const char* filename);

// 读取文件中的数据
void loadTransactionsFromFile(const char* filename);


#endif /* TRANSACTION_H */