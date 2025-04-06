#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "main.h"


void initTransactionList();


Transaction* add_Transaction(int type, int subType, double amount, const char* description);

Transaction** getTransactionsByDateRange(const char* startDate, const char* endDate, int* count);

// ����
double calculateTotalIncome(const char* startDate, const char* endDate);

// ֧��
double calculateTotalExpense(const char* startDate, const char* endDate);

// �����ݵ������ļ���
void saveTransactionsToFile(const char* filename);

// ��ȡ�ļ��е�����
void loadTransactionsFromFile(const char* filename);


#endif /* TRANSACTION_H */