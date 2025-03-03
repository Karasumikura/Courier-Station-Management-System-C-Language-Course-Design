#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "main.h"

// 初始化交易记录链表
void initTransactionList();

// 添加交易记录
Transaction* addTransaction(int type, int subType, double amount, const char* description);

// 查找交易记录（通过ID）
Transaction* findTransactionById(int transactionId);

// 查询时间范围内的交易记录
Transaction** getTransactionsByDateRange(const char* startDate, const char* endDate, int* count);

// 生成日报
void generateDailyReport(const char* date, char* reportOutput);

// 生成周报
void generateWeeklyReport(const char* startDate, char* reportOutput);

// 生成月报
void generateMonthlyReport(const char* yearMonth, char* reportOutput);

// 分析盈利趋势
void analyzeProfitTrend(const char* startDate, const char* endDate, char* analysisOutput);

// 保存交易记录到文件
void saveTransactionsToFile(const char* filename);

// 从文件加载交易记录
void loadTransactionsFromFile(const char* filename);

// 释放交易记录链表内存
void freeTransactionList();

#endif /* TRANSACTION_H */