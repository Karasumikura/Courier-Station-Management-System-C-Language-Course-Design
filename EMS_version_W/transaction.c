#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "transaction.h"
#include "util.h"

// ȫ�ֽ��׼�¼����
Transaction* g_transactionList = NULL;

// ��ʼ�����׼�¼����
void initTransactionList() {
    g_transactionList = NULL;
}

// ��ӽ��׼�¼
Transaction* add_Transaction(int type, int subType, double amount, const char* description) {
    // �����½��׼�¼
    Transaction* newTransaction = (Transaction*)malloc(sizeof(Transaction));
    if (newTransaction == NULL) {
        return NULL;
    }

    // ��ʼ�����׼�¼����
    newTransaction->id = generateUniqueId();
    newTransaction->type = type;
    newTransaction->subType = subType;
    newTransaction->amount = amount;
    strncpy(newTransaction->description, description, sizeof(newTransaction->description) - 1);
    newTransaction->description[sizeof(newTransaction->description) - 1] = '\0';

    // ��ȡ��ǰʱ��
    getCurrentTimeString(newTransaction->createTime);

    // ���뵽����
    newTransaction->next = g_transactionList;
    g_transactionList = newTransaction;

    return newTransaction;
}

// ��ȡָ�����ڷ�Χ�ڵĽ��׼�¼
Transaction** getTransactionsByDateRange(const char* startDate, const char* endDate, int* count) {
    // ������������Ľ��׼�¼����
    *count = 0;
    Transaction* current = g_transactionList;

    while (current != NULL) {
        // ��������Ƿ��ڷ�Χ��
        // �����ʵ�֣�ʵ����Ӧ�ý������ڽ��бȽ�
        if (strcmp(current->createTime, startDate) >= 0 &&
            strcmp(current->createTime, endDate) <= 0) {
            (*count)++;
        }
        current = current->next;
    }

    if (*count == 0) {
        return NULL;
    }

    // �����ڴ�
    Transaction** transactions = (Transaction**)malloc(sizeof(Transaction*) * (*count));
    if (transactions == NULL) {
        *count = 0;
        return NULL;
    }

    // �������
    current = g_transactionList;
    int index = 0;

    while (current != NULL && index < *count) {
        if (strcmp(current->createTime, startDate) >= 0 &&
            strcmp(current->createTime, endDate) <= 0) {
            transactions[index++] = current;
        }
        current = current->next;
    }

    return transactions;
}

// ����ָ�����ڷ�Χ�ڵ������ܶ�
double calculateTotalIncome(const char* startDate, const char* endDate) {
    int count;
    Transaction** transactions = getTransactionsByDateRange(startDate, endDate, &count);

    if (transactions == NULL) {
        return 0.0;
    }

    double totalIncome = 0.0;
    for (int i = 0; i < count; i++) {
        if (transactions[i]->type == TRANSACTION_INCOME) {
            totalIncome += transactions[i]->amount;
        }
    }

    free(transactions);
    return totalIncome;
}

// ����ָ�����ڷ�Χ�ڵ�֧���ܶ�
double calculateTotalExpense(const char* startDate, const char* endDate) {
    int count;
    Transaction** transactions = getTransactionsByDateRange(startDate, endDate, &count);

    if (transactions == NULL) {
        return 0.0;
    }

    double totalExpense = 0.0;
    for (int i = 0; i < count; i++) {
        if (transactions[i]->type == TRANSACTION_EXPENSE) {
            totalExpense += transactions[i]->amount;
        }
    }

    free(transactions);
    return totalExpense;
}

// ���潻�׼�¼���ݵ��ļ�
void saveTransactionsToFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("�޷��򿪽��׼�¼�����ļ� %s\n", filename);
        return;
    }

    Transaction* current = g_transactionList;
    while (current != NULL) {
        fprintf(file, "%d,%d,%d,%.2f,%s,%s\n",
            current->id,
            current->type,
            current->subType,
            current->amount,
            current->description,
            current->createTime);
        current = current->next;
    }

    fclose(file);
}

// ���ļ����ؽ��׼�¼����
void loadTransactionsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("���׼�¼�����ļ� %s �����ڣ����������ļ�\n", filename);
    }

    // �����������
    while (g_transactionList != NULL) {
        Transaction* temp = g_transactionList;
        g_transactionList = g_transactionList->next;
        free(temp);
    }

    // ��ȡ�ļ�����
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        int id, type, subType;
        double amount;
        char description[100], createTime[20];

        if (sscanf(line, "%d,%d,%d,%lf,%[^,],%[^\n]",
            &id, &type, &subType, &amount, description, createTime) == 6) {

            // �����½��׼�¼�ڵ�
            Transaction* newTransaction = (Transaction*)malloc(sizeof(Transaction));
            if (newTransaction != NULL) {
                newTransaction->id = id;
                newTransaction->type = type;
                newTransaction->subType = subType;
                newTransaction->amount = amount;
                strncpy(newTransaction->description, description, sizeof(newTransaction->description) - 1);
                newTransaction->description[sizeof(newTransaction->description) - 1] = '\0';
                strncpy(newTransaction->createTime, createTime, sizeof(newTransaction->createTime) - 1);
                newTransaction->createTime[sizeof(newTransaction->createTime) - 1] = '\0';

                // ���뵽����ͷ��
                newTransaction->next = g_transactionList;
                g_transactionList = newTransaction;
            }
        }
    }

    fclose(file);
}