#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "main.h"

// ��ʼ�����׼�¼����
void initTransactionList();

// ��ӽ��׼�¼
Transaction* addTransaction(int type, int subType, double amount, const char* description);

// ���ҽ��׼�¼��ͨ��ID��
Transaction* findTransactionById(int transactionId);

// ��ѯʱ�䷶Χ�ڵĽ��׼�¼
Transaction** getTransactionsByDateRange(const char* startDate, const char* endDate, int* count);

// �����ձ�
void generateDailyReport(const char* date, char* reportOutput);

// �����ܱ�
void generateWeeklyReport(const char* startDate, char* reportOutput);

// �����±�
void generateMonthlyReport(const char* yearMonth, char* reportOutput);

// ����ӯ������
void analyzeProfitTrend(const char* startDate, const char* endDate, char* analysisOutput);

// ���潻�׼�¼���ļ�
void saveTransactionsToFile(const char* filename);

// ���ļ����ؽ��׼�¼
void loadTransactionsFromFile(const char* filename);

// �ͷŽ��׼�¼�����ڴ�
void freeTransactionList();

#endif /* TRANSACTION_H */