#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "transaction.h"
#include "util.h"

// 全局交易记录链表
Transaction* g_transactionList = NULL;

// 初始化交易记录链表
void initTransactionList() {
    g_transactionList = NULL;
}

// 添加交易记录
Transaction* add_Transaction(int type, int subType, double amount, const char* description) {
    // 创建新交易记录
    Transaction* newTransaction = (Transaction*)malloc(sizeof(Transaction));
    if (newTransaction == NULL) {
        return NULL;
    }

    // 初始化交易记录数据
    newTransaction->id = generateUniqueId();
    newTransaction->type = type;
    newTransaction->subType = subType;
    newTransaction->amount = amount;
    strncpy(newTransaction->description, description, sizeof(newTransaction->description) - 1);
    newTransaction->description[sizeof(newTransaction->description) - 1] = '\0';

    // 获取当前时间
    getCurrentTimeString(newTransaction->createTime);

    // 插入到链表
    newTransaction->next = g_transactionList;
    g_transactionList = newTransaction;

    return newTransaction;
}

// 获取指定日期范围内的交易记录
Transaction** getTransactionsByDateRange(const char* startDate, const char* endDate, int* count) {
    // 计算符合条件的交易记录数量
    *count = 0;
    Transaction* current = g_transactionList;

    while (current != NULL) {
        // 检查日期是否在范围内
        // 这里简化实现，实际上应该解析日期进行比较
        if (strcmp(current->createTime, startDate) >= 0 &&
            strcmp(current->createTime, endDate) <= 0) {
            (*count)++;
        }
        current = current->next;
    }

    if (*count == 0) {
        return NULL;
    }

    // 分配内存
    Transaction** transactions = (Transaction**)malloc(sizeof(Transaction*) * (*count));
    if (transactions == NULL) {
        *count = 0;
        return NULL;
    }

    // 填充数组
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

// 计算指定日期范围内的收入总额
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

// 计算指定日期范围内的支出总额
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

// 保存交易记录数据到文件
void saveTransactionsToFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法打开交易记录数据文件 %s\n", filename);
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

// 从文件加载交易记录数据
void loadTransactionsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("交易记录数据文件 %s 不存在，将创建新文件\n", filename);
    }

    // 清空现有链表
    while (g_transactionList != NULL) {
        Transaction* temp = g_transactionList;
        g_transactionList = g_transactionList->next;
        free(temp);
    }

    // 读取文件数据
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        int id, type, subType;
        double amount;
        char description[100], createTime[20];

        if (sscanf(line, "%d,%d,%d,%lf,%[^,],%[^\n]",
            &id, &type, &subType, &amount, description, createTime) == 6) {

            // 创建新交易记录节点
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

                // 插入到链表头部
                newTransaction->next = g_transactionList;
                g_transactionList = newTransaction;
            }
        }
    }

    fclose(file);
}