#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "transaction.h"
#include "util.h"

//交易记录的相关功能
Transaction* g_transactionList = NULL;


void initTransactionList() {
    g_transactionList = NULL;
}


Transaction* add_Transaction(int type, int subType, double amount, const char* description) {
    
    Transaction* newTransaction = (Transaction*)malloc(sizeof(Transaction));
    if (newTransaction == NULL) {
        return NULL;
    }

    
    newTransaction->id = generateUniqueId();
    newTransaction->type = type;
    newTransaction->subType = subType;
    newTransaction->amount = amount;
    strncpy(newTransaction->description, description, sizeof(newTransaction->description) - 1);
    newTransaction->description[sizeof(newTransaction->description) - 1] = '\0';

    
    getCurrentTimeString(newTransaction->createTime);

    
    newTransaction->next = g_transactionList;
    g_transactionList = newTransaction;

    return newTransaction;
}


Transaction** getTransactionsByDateRange(const char* startDate, const char* endDate, int* count) {
    
    *count = 0;
    Transaction* current = g_transactionList;
    
    while (current != NULL) {
        if (current->createTime == NULL || startDate == NULL || endDate == NULL) {
            continue; // 跳过无效记录
        }
        if (strcmp(current->createTime, startDate) >= 0 &&
            strcmp(current->createTime, endDate) <= 0) {
            (*count)++;
        }
        current = current->next;
    }

    if (*count == 0) {
        return NULL;
    }

    
    Transaction** transactions = (Transaction**)malloc(sizeof(Transaction*) * (*count));
    if (transactions == NULL) {
        *count = 0;
        return NULL;
    }

    
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

// 收入
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

// 支出
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

// 将数据导出到文件中
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

// 读取文件的数据
void loadTransactionsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("交易记录数据文件 %s 不存在，将创建新文件\n", filename);
    }

    
    while (g_transactionList != NULL) {
        Transaction* temp = g_transactionList;
        g_transactionList = g_transactionList->next;
        free(temp);
    }

    
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        int id, type, subType;
        double amount;
        char description[100], createTime[20];

        if (sscanf(line, "%d,%d,%d,%lf,%[^,],%[^\n]",
            &id, &type, &subType, &amount, description, createTime) == 6) {

            
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

                
                newTransaction->next = g_transactionList;
                g_transactionList = newTransaction;
            }
        }
    }

    fclose(file);
}

void printtransaction() {
	int count = 0;
    printf("请输入开始日期\n");
    char* date = timeinput();
    if (date == NULL) {
        printf("日期格式无效！\n");
        waitForKeyPress();
        return;
    }
    int dateinput;
    printf("请接下来输入所需时间范围\n");
    if (scanf("%d", &dateinput) == 0) {
        printf("日期格式无效！\n");
        waitForKeyPress();
        return;
    }
    char *date2 = getNexttime(date, dateinput);
    Transaction** transactions = getTransactionsByDateRange(date, date2, &count);//用*int获取交易记录数量
    if (transactions == NULL) printf("没有交易记录！\n");
    else {
        printf("找到 %d 条交易记录：\n", count);
        for (int i = 0; i < count; i++) {
            printf("交易 ID：%d，类型：%s，金额：%.2f元\n",
                transactions[i]->id,
                transactions[i]->type == TRANSACTION_INCOME ? "收入" : "支出",
                transactions[i]->amount);
        }
    }
    waitForKeyPress();
    return;
}