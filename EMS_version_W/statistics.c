#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "main.h"
#include "statistics.h"
#include "transaction.h"
#include "package.h"
#include "shelf.h"
#include "util.h"
#include "user.h"

// 生成日报
void generateDailyReport(char* reportOutput) {
    char* date = timeinput();
    if (date == NULL) {
		printf("日期错误,请重试");
        waitForKeyPress();
		return;
    }
    char* date2 =getNextDay(date);
	if (date2 == NULL) {
		printf("日期错误,请重试");
        waitForKeyPress();
		return;
	}
    // 获取指定日期的所有交易记录
    int count = 0;
    Transaction** transactions = getTransactionsByDateRange(date, date2, &count);

    if (transactions == NULL || count == 0) {
        sprintf(reportOutput, "日期: %s\n没有交易记录。", date);
        printf("%s", reportOutput);
        waitForKeyPress();
        return;
    }

    // 统计收入和支出
    double totalIncome = 0.0;
    double totalExpense = 0.0;
    int packageCount = 0;

    for (int i = 0; i < count; i++) {
        if (transactions[i]->type == TRANSACTION_INCOME) {
            totalIncome += transactions[i]->amount;
            if (transactions[i]->subType == INCOME_PIECE_FEE) {
                packageCount++;
            }
        }
        else {
            totalExpense += transactions[i]->amount;
        }
    }

    // 生成报告
    sprintf(reportOutput,
        "===== 日报：%s =====\n"
        "处理包裹数: %d\n"
        "总收入: %.2f元\n"
        "总支出: %.2f元\n"
        "净利润: %.2f元\n",
        date, packageCount, totalIncome, totalExpense, totalIncome - totalExpense);
	printf("日报生成成功！\n");
    printf("%s", reportOutput);
	waitForKeyPress();
    free(transactions);
}

// 生成周报
void generateWeeklyReport(const char* startDate, const char* endDate, char* reportOutput) {
    // 计算日期范围内的总收入和总支出
    double totalIncome = calculateTotalIncome(startDate, endDate);
    double totalExpense = calculateTotalExpense(startDate, endDate);

    // 获取包裹处理数量
    int packageCount = 0;
    int count = 0;
    Transaction** transactions = getTransactionsByDateRange(startDate, endDate, &count);

    if (transactions != NULL) {
        for (int i = 0; i < count; i++) {
            if (transactions[i]->type == TRANSACTION_INCOME &&
                transactions[i]->subType == INCOME_PIECE_FEE) {
                packageCount++;
            }
        }
        free(transactions);
    }

    // 生成报告
    sprintf(reportOutput,
        "===== 周报：%s 至 %s =====\n"
        "处理包裹数: %d\n"
        "总收入: %.2f元\n"
        "总支出: %.2f元\n"
        "净利润: %.2f元\n"
        "日均包裹处理量: %.1f\n",
        startDate, endDate, packageCount, totalIncome, totalExpense,
        totalIncome - totalExpense, packageCount / 7.0);
}

// 生成月报
void generateMonthlyReport(const char* month, char* reportOutput) {
    // 这里简化实现，假设month格式为"YYYY-MM"
    char startDate[11], endDate[11];
    sprintf(startDate, "%s-01", month);
    sprintf(endDate, "%s-31", month);  // 简化处理，使用31作为月末

    // 计算日期范围内的总收入和总支出
    double totalIncome = calculateTotalIncome(startDate, endDate);
    double totalExpense = calculateTotalExpense(startDate, endDate);

    // 获取包裹处理数量和异常数量
    int packageCount = 0;
    int abnormalCount = 0;
    int count = 0;
    Transaction** transactions = getTransactionsByDateRange(startDate, endDate, &count);

    if (transactions != NULL) {
        for (int i = 0; i < count; i++) {
            if (transactions[i]->type == TRANSACTION_INCOME &&
                transactions[i]->subType == INCOME_PIECE_FEE) {
                packageCount++;
            }
            else if (transactions[i]->type == TRANSACTION_EXPENSE &&
                transactions[i]->subType == EXPENSE_COMPENSATION) {
                abnormalCount++;
            }
        }
        free(transactions);
    }

    // 获取货架利用情况
    char shelfStatus[1024];
    getShelvesStatus(shelfStatus);

    // 生成报告
    sprintf(reportOutput,
        "===== 月报：%s =====\n"
        "处理包裹数: %d\n"
        "异常包裹数: %d\n"
        "总收入: %.2f元\n"
        "总支出: %.2f元\n"
        "净利润: %.2f元\n"
        "日均包裹处理量: %.1f\n\n"
        "货架使用情况:\n%s",
        month, packageCount, abnormalCount, totalIncome, totalExpense,
        totalIncome - totalExpense, packageCount / 30.0, shelfStatus);
}

// 包裹流量分析
void analyzePackageFlow(char* analysisOutput) {
    // 获取当前时间
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);

    char currentMonth[8];
    strftime(currentMonth, sizeof(currentMonth), "%Y-%m", tm_info);

    char prevMonth[8];
    tm_info->tm_mon -= 1;
    if (tm_info->tm_mon < 0) {
        tm_info->tm_mon = 11;
        tm_info->tm_year -= 1;
    }
    strftime(prevMonth, sizeof(prevMonth), "%Y-%m", tm_info);

    // 当前月起止日期
    char currentMonthStart[11], currentMonthEnd[11];
    sprintf(currentMonthStart, "%s-01", currentMonth);
    sprintf(currentMonthEnd, "%s-31", currentMonth);

    // 上个月起止日期
    char prevMonthStart[11], prevMonthEnd[11];
    sprintf(prevMonthStart, "%s-01", prevMonth);
    sprintf(prevMonthEnd, "%s-31", prevMonth);

    // 获取当前月包裹数量
    int currentMonthCount = 0;
    int count = 0;
    Transaction** transactions = getTransactionsByDateRange(currentMonthStart, currentMonthEnd, &count);

    if (transactions != NULL) {
        for (int i = 0; i < count; i++) {
            if (transactions[i]->type == TRANSACTION_INCOME &&
                transactions[i]->subType == INCOME_PIECE_FEE) {
                currentMonthCount++;
            }
        }
        free(transactions);
    }

    // 获取上个月包裹数量
    int prevMonthCount = 0;
    transactions = getTransactionsByDateRange(prevMonthStart, prevMonthEnd, &count);

    if (transactions != NULL) {
        for (int i = 0; i < count; i++) {
            if (transactions[i]->type == TRANSACTION_INCOME &&
                transactions[i]->subType == INCOME_PIECE_FEE) {
                prevMonthCount++;
            }
        }
        free(transactions);
    }

    // 计算增长率
    float growthRate = 0;
    if (prevMonthCount > 0) {
        growthRate = (float)(currentMonthCount - prevMonthCount) / prevMonthCount * 100;
    }

    // 生成分析报告
    sprintf(analysisOutput,
        "===== 包裹流量分析 =====\n"
        "当前月份: %s, 包裹数量: %d\n"
        "上个月份: %s, 包裹数量: %d\n"
        "环比增长率: %.1f%%\n"
        "日均包裹处理量: %.1f\n",
        currentMonth, currentMonthCount,
        prevMonth, prevMonthCount,
        growthRate,
        (float)currentMonthCount / 30);
}

// 收入分析
void analyzeIncome(char* analysisOutput) {
    // 获取当前时间
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);

    char currentMonth[8];
    strftime(currentMonth, sizeof(currentMonth), "%Y-%m", tm_info);

    // 当前月起止日期
    char currentMonthStart[11], currentMonthEnd[11];
    sprintf(currentMonthStart, "%s-01", currentMonth);
    sprintf(currentMonthEnd, "%s-31", currentMonth);

    // 获取总收入数据
    double totalIncome = calculateTotalIncome(currentMonthStart, currentMonthEnd);

    // 获取各类收入明细
    int count = 0;
    Transaction** transactions = getTransactionsByDateRange(currentMonthStart, currentMonthEnd, &count);

    double pieceFeeIncome = 0.0;
    double storageFeeIncome = 0.0;
    double otherIncome = 0.0;

    if (transactions != NULL) {
        for (int i = 0; i < count; i++) {
            if (transactions[i]->type == TRANSACTION_INCOME) {
                switch (transactions[i]->subType) {
                case INCOME_PIECE_FEE:
                    pieceFeeIncome += transactions[i]->amount;
                    break;
                case INCOME_STORAGE_FEE:
                    storageFeeIncome += transactions[i]->amount;
                    break;
                default:
                    otherIncome += transactions[i]->amount;
                }
            }
        }
        free(transactions);
    }

    // 计算各类收入占比
    float pieceFeePercent = totalIncome > 0 ? (pieceFeeIncome / totalIncome * 100) : 0;
    float storageFeePercent = totalIncome > 0 ? (storageFeeIncome / totalIncome * 100) : 0;
    float otherPercent = totalIncome > 0 ? (otherIncome / totalIncome * 100) : 0;

    // 生成分析报告
    sprintf(analysisOutput,
        "===== 收入分析：%s =====\n"
        "总收入: %.2f元\n\n"
        "收入明细:\n"
        "计件费收入: %.2f元 (%.1f%%)\n"
        "保存费收入: %.2f元 (%.1f%%)\n"
        "其他收入: %.2f元 (%.1f%%)\n",
        currentMonth, totalIncome,
        pieceFeeIncome, pieceFeePercent,
        storageFeeIncome, storageFeePercent,
        otherIncome, otherPercent);
}

// 寻找最优货架存放
void optimizeShelfPlacement(char* optimizationOutput) {
    // 获取所有货架使用情况
    Shelf* current = g_shelfList;
    int shelfCount = 0;
    float* utilizations = NULL;
    int* shelfIds = NULL;

    // 计算货架数量
    while (current != NULL) {
        shelfCount++;
        current = current->next;
    }

    if (shelfCount == 0) {
        sprintf(optimizationOutput, "系统中未配置货架！");
        return;
    }

    // 分配内存
    utilizations = (float*)malloc(sizeof(float) * shelfCount);
    shelfIds = (int*)malloc(sizeof(int) * shelfCount);

    if (utilizations == NULL || shelfIds == NULL) {
        sprintf(optimizationOutput, "内存分配失败！");
        if (utilizations) free(utilizations);
        if (shelfIds) free(shelfIds);
        return;
    }

    // 获取每个货架的使用率
    current = g_shelfList;
    for (int i = 0; i < shelfCount; i++) {
        utilizations[i] = (float)current->currentCount / current->capacity;
        shelfIds[i] = current->id;
        current = current->next;
    }

    // 简单的排序来找出使用率最高和最低的货架
    for (int i = 0; i < shelfCount - 1; i++) {
        for (int j = 0; j < shelfCount - i - 1; j++) {
            if (utilizations[j] < utilizations[j + 1]) {
                // 交换使用率
                float tempUtil = utilizations[j];
                utilizations[j] = utilizations[j + 1];
                utilizations[j + 1] = tempUtil;

                // 交换ID
                int tempId = shelfIds[j];
                shelfIds[j] = shelfIds[j + 1];
                shelfIds[j + 1] = tempId;
            }
        }
    }

    // 生成优化建议
    sprintf(optimizationOutput,
        "===== 货架存放优化建议 =====\n\n"
        "利用率最高的货架:\n");

    for (int i = 0; i < 3 && i < shelfCount; i++) {
        Shelf* shelf = findShelfById(shelfIds[i]);
        char typeStr[20];

        switch (shelf->type) {
        case 0: strcpy(typeStr, "极小包裹"); break;
        case 1: strcpy(typeStr, "小包裹"); break;
        case 2: strcpy(typeStr, "中包裹"); break;
        case 3: strcpy(typeStr, "大包裹"); break;
        case 4: strcpy(typeStr, "极大包裹"); break;
        case 5: strcpy(typeStr, "易碎品"); break;
        case 6: strcpy(typeStr, "冷鲜"); break;
        default: strcpy(typeStr, "未知");
        }

        char tempStr[100];
        sprintf(tempStr, "货架ID: %d, 类型: %s, 使用率: %.1f%%\n",
            shelf->id, typeStr, utilizations[i] * 100);
        strcat(optimizationOutput, tempStr);
    }

    strcat(optimizationOutput, "\n利用率最低的货架:\n");

    for (int i = shelfCount - 1; i >= shelfCount - 3 && i >= 0; i--) {
        Shelf* shelf = findShelfById(shelfIds[i]);
        char typeStr[20];

        switch (shelf->type) {
        case 0: strcpy(typeStr, "极小包裹"); break;
        case 1: strcpy(typeStr, "小包裹"); break;
        case 2: strcpy(typeStr, "中包裹"); break;
        case 3: strcpy(typeStr, "大包裹"); break;
        case 4: strcpy(typeStr, "极大包裹"); break;
        case 5: strcpy(typeStr, "易碎品"); break;
        case 6: strcpy(typeStr, "冷鲜"); break;
        default: strcpy(typeStr, "未知");
        }

        char tempStr[100];
        sprintf(tempStr, "货架ID: %d, 类型: %s, 使用率: %.1f%%\n",
            shelf->id, typeStr, utilizations[i] * 100);
        strcat(optimizationOutput, tempStr);
    }

    // 添加优化建议
    strcat(optimizationOutput, "\n优化建议:\n");

    // 如果某类货架使用率高，建议增加
    for (int i = 0; i < 3 && i < shelfCount; i++) {
        if (utilizations[i] > 0.8) {  // 使用率超过80%
            Shelf* shelf = findShelfById(shelfIds[i]);
            char typeStr[20];

            switch (shelf->type) {
            case 0: strcpy(typeStr, "极小包裹"); break;
            case 1: strcpy(typeStr, "小包裹"); break;
            case 2: strcpy(typeStr, "中包裹"); break;
            case 3: strcpy(typeStr, "大包裹"); break;
            case 4: strcpy(typeStr, "极大包裹"); break;
            case 5: strcpy(typeStr, "易碎品"); break;
            case 6: strcpy(typeStr, "冷鲜"); break;
            default: strcpy(typeStr, "未知");
            }

            char tempStr[100];
            sprintf(tempStr, "- 建议增加%s类型货架的容量\n", typeStr);
            strcat(optimizationOutput, tempStr);
        }
    }

    // 如果某类货架使用率低，建议减少
    for (int i = shelfCount - 1; i >= shelfCount - 3 && i >= 0; i--) {
        if (utilizations[i] < 0.2) {  // 使用率低于20%
            Shelf* shelf = findShelfById(shelfIds[i]);
            char typeStr[20];

            switch (shelf->type) {
            case 0: strcpy(typeStr, "极小包裹"); break;
            case 1: strcpy(typeStr, "小包裹"); break;
            case 2: strcpy(typeStr, "中包裹"); break;
            case 3: strcpy(typeStr, "大包裹"); break;
            case 4: strcpy(typeStr, "极大包裹"); break;
            case 5: strcpy(typeStr, "易碎品"); break;
            case 6: strcpy(typeStr, "冷鲜"); break;
            default: strcpy(typeStr, "未知");
            }

            char tempStr[100];
            sprintf(tempStr, "- 可以减少%s类型货架的数量\n", typeStr);
            strcat(optimizationOutput, tempStr);
        }
    }

    // 释放内存
    free(utilizations);
    free(shelfIds);
}