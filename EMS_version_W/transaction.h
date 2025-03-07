#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "main.h"

// ��ʼ�����׼�¼����
void initTransactionList();

// ��ӽ��׼�¼
Transaction* add_Transaction(int type, int subType, double amount, const char* description);

// ���ҽ��׼�¼��ͨ��ID��
Transaction* findTransactionById(int transactionId);

// ��ѯʱ�䷶Χ�ڵĽ��׼�¼
Transaction** getTransactionsByDateRange(const char* startDate, const char* endDate, int* count);

// ����ָ�����ڷ�Χ�ڵ������ܶ�
double calculateTotalIncome(const char* startDate, const char* endDate);

// ����ָ�����ڷ�Χ�ڵ�֧���ܶ�
double calculateTotalExpense(const char* startDate, const char* endDate);

// ���潻�׼�¼���ļ�
void saveTransactionsToFile(const char* filename);

// ���ļ����ؽ��׼�¼
void loadTransactionsFromFile(const char* filename);

// �ͷŽ��׼�¼�����ڴ�
void freeTransactionList();


#endif /* TRANSACTION_H */