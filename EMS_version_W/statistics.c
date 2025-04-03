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

// �����ձ�
void generateDailyReport(char* reportOutput) {
    char* date = timeinput();
    if (date == NULL) {
		printf("���ڴ���,������");
        waitForKeyPress();
		return;
    }
    char* date2 =getNextDay(date);
	if (date2 == NULL) {
		printf("���ڴ���,������");
        waitForKeyPress();
		return;
	}
    // ��ȡָ�����ڵ����н��׼�¼
    int count = 0;
    Transaction** transactions = getTransactionsByDateRange(date, date2, &count);

    if (transactions == NULL || count == 0) {
        sprintf(reportOutput, "����: %s\nû�н��׼�¼��", date);
        printf("%s", reportOutput);
        waitForKeyPress();
        return;
    }

    // ͳ�������֧��
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

    // ���ɱ���
    sprintf(reportOutput,
        "===== �ձ���%s =====\n"
        "���������: %d\n"
        "������: %.2fԪ\n"
        "��֧��: %.2fԪ\n"
        "������: %.2fԪ\n",
        date, packageCount, totalIncome, totalExpense, totalIncome - totalExpense);
	printf("�ձ����ɳɹ���\n");
    printf("%s", reportOutput);
	waitForKeyPress();
    free(transactions);
}

// �����ܱ�
void generateWeeklyReport(const char* startDate, const char* endDate, char* reportOutput) {
    // �������ڷ�Χ�ڵ����������֧��
    double totalIncome = calculateTotalIncome(startDate, endDate);
    double totalExpense = calculateTotalExpense(startDate, endDate);

    // ��ȡ������������
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

    // ���ɱ���
    sprintf(reportOutput,
        "===== �ܱ���%s �� %s =====\n"
        "���������: %d\n"
        "������: %.2fԪ\n"
        "��֧��: %.2fԪ\n"
        "������: %.2fԪ\n"
        "�վ�����������: %.1f\n",
        startDate, endDate, packageCount, totalIncome, totalExpense,
        totalIncome - totalExpense, packageCount / 7.0);
}

// �����±�
void generateMonthlyReport(const char* month, char* reportOutput) {
    // �����ʵ�֣�����month��ʽΪ"YYYY-MM"
    char startDate[11], endDate[11];
    sprintf(startDate, "%s-01", month);
    sprintf(endDate, "%s-31", month);  // �򻯴���ʹ��31��Ϊ��ĩ

    // �������ڷ�Χ�ڵ����������֧��
    double totalIncome = calculateTotalIncome(startDate, endDate);
    double totalExpense = calculateTotalExpense(startDate, endDate);

    // ��ȡ���������������쳣����
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

    // ��ȡ�����������
    char shelfStatus[1024];
    getShelvesStatus(shelfStatus);

    // ���ɱ���
    sprintf(reportOutput,
        "===== �±���%s =====\n"
        "���������: %d\n"
        "�쳣������: %d\n"
        "������: %.2fԪ\n"
        "��֧��: %.2fԪ\n"
        "������: %.2fԪ\n"
        "�վ�����������: %.1f\n\n"
        "����ʹ�����:\n%s",
        month, packageCount, abnormalCount, totalIncome, totalExpense,
        totalIncome - totalExpense, packageCount / 30.0, shelfStatus);
}

// ������������
void analyzePackageFlow(char* analysisOutput) {
    // ��ȡ��ǰʱ��
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

    // ��ǰ����ֹ����
    char currentMonthStart[11], currentMonthEnd[11];
    sprintf(currentMonthStart, "%s-01", currentMonth);
    sprintf(currentMonthEnd, "%s-31", currentMonth);

    // �ϸ�����ֹ����
    char prevMonthStart[11], prevMonthEnd[11];
    sprintf(prevMonthStart, "%s-01", prevMonth);
    sprintf(prevMonthEnd, "%s-31", prevMonth);

    // ��ȡ��ǰ�°�������
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

    // ��ȡ�ϸ��°�������
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

    // ����������
    float growthRate = 0;
    if (prevMonthCount > 0) {
        growthRate = (float)(currentMonthCount - prevMonthCount) / prevMonthCount * 100;
    }

    // ���ɷ�������
    sprintf(analysisOutput,
        "===== ������������ =====\n"
        "��ǰ�·�: %s, ��������: %d\n"
        "�ϸ��·�: %s, ��������: %d\n"
        "����������: %.1f%%\n"
        "�վ�����������: %.1f\n",
        currentMonth, currentMonthCount,
        prevMonth, prevMonthCount,
        growthRate,
        (float)currentMonthCount / 30);
}

// �������
void analyzeIncome(char* analysisOutput) {
    // ��ȡ��ǰʱ��
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);

    char currentMonth[8];
    strftime(currentMonth, sizeof(currentMonth), "%Y-%m", tm_info);

    // ��ǰ����ֹ����
    char currentMonthStart[11], currentMonthEnd[11];
    sprintf(currentMonthStart, "%s-01", currentMonth);
    sprintf(currentMonthEnd, "%s-31", currentMonth);

    // ��ȡ����������
    double totalIncome = calculateTotalIncome(currentMonthStart, currentMonthEnd);

    // ��ȡ����������ϸ
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

    // �����������ռ��
    float pieceFeePercent = totalIncome > 0 ? (pieceFeeIncome / totalIncome * 100) : 0;
    float storageFeePercent = totalIncome > 0 ? (storageFeeIncome / totalIncome * 100) : 0;
    float otherPercent = totalIncome > 0 ? (otherIncome / totalIncome * 100) : 0;

    // ���ɷ�������
    sprintf(analysisOutput,
        "===== ���������%s =====\n"
        "������: %.2fԪ\n\n"
        "������ϸ:\n"
        "�Ƽ�������: %.2fԪ (%.1f%%)\n"
        "���������: %.2fԪ (%.1f%%)\n"
        "��������: %.2fԪ (%.1f%%)\n",
        currentMonth, totalIncome,
        pieceFeeIncome, pieceFeePercent,
        storageFeeIncome, storageFeePercent,
        otherIncome, otherPercent);
}

// Ѱ�����Ż��ܴ��
void optimizeShelfPlacement(char* optimizationOutput) {
    // ��ȡ���л���ʹ�����
    Shelf* current = g_shelfList;
    int shelfCount = 0;
    float* utilizations = NULL;
    int* shelfIds = NULL;

    // �����������
    while (current != NULL) {
        shelfCount++;
        current = current->next;
    }

    if (shelfCount == 0) {
        sprintf(optimizationOutput, "ϵͳ��δ���û��ܣ�");
        return;
    }

    // �����ڴ�
    utilizations = (float*)malloc(sizeof(float) * shelfCount);
    shelfIds = (int*)malloc(sizeof(int) * shelfCount);

    if (utilizations == NULL || shelfIds == NULL) {
        sprintf(optimizationOutput, "�ڴ����ʧ�ܣ�");
        if (utilizations) free(utilizations);
        if (shelfIds) free(shelfIds);
        return;
    }

    // ��ȡÿ�����ܵ�ʹ����
    current = g_shelfList;
    for (int i = 0; i < shelfCount; i++) {
        utilizations[i] = (float)current->currentCount / current->capacity;
        shelfIds[i] = current->id;
        current = current->next;
    }

    // �򵥵��������ҳ�ʹ������ߺ���͵Ļ���
    for (int i = 0; i < shelfCount - 1; i++) {
        for (int j = 0; j < shelfCount - i - 1; j++) {
            if (utilizations[j] < utilizations[j + 1]) {
                // ����ʹ����
                float tempUtil = utilizations[j];
                utilizations[j] = utilizations[j + 1];
                utilizations[j + 1] = tempUtil;

                // ����ID
                int tempId = shelfIds[j];
                shelfIds[j] = shelfIds[j + 1];
                shelfIds[j + 1] = tempId;
            }
        }
    }

    // �����Ż�����
    sprintf(optimizationOutput,
        "===== ���ܴ���Ż����� =====\n\n"
        "��������ߵĻ���:\n");

    for (int i = 0; i < 3 && i < shelfCount; i++) {
        Shelf* shelf = findShelfById(shelfIds[i]);
        char typeStr[20];

        switch (shelf->type) {
        case 0: strcpy(typeStr, "��С����"); break;
        case 1: strcpy(typeStr, "С����"); break;
        case 2: strcpy(typeStr, "�а���"); break;
        case 3: strcpy(typeStr, "�����"); break;
        case 4: strcpy(typeStr, "�������"); break;
        case 5: strcpy(typeStr, "����Ʒ"); break;
        case 6: strcpy(typeStr, "����"); break;
        default: strcpy(typeStr, "δ֪");
        }

        char tempStr[100];
        sprintf(tempStr, "����ID: %d, ����: %s, ʹ����: %.1f%%\n",
            shelf->id, typeStr, utilizations[i] * 100);
        strcat(optimizationOutput, tempStr);
    }

    strcat(optimizationOutput, "\n��������͵Ļ���:\n");

    for (int i = shelfCount - 1; i >= shelfCount - 3 && i >= 0; i--) {
        Shelf* shelf = findShelfById(shelfIds[i]);
        char typeStr[20];

        switch (shelf->type) {
        case 0: strcpy(typeStr, "��С����"); break;
        case 1: strcpy(typeStr, "С����"); break;
        case 2: strcpy(typeStr, "�а���"); break;
        case 3: strcpy(typeStr, "�����"); break;
        case 4: strcpy(typeStr, "�������"); break;
        case 5: strcpy(typeStr, "����Ʒ"); break;
        case 6: strcpy(typeStr, "����"); break;
        default: strcpy(typeStr, "δ֪");
        }

        char tempStr[100];
        sprintf(tempStr, "����ID: %d, ����: %s, ʹ����: %.1f%%\n",
            shelf->id, typeStr, utilizations[i] * 100);
        strcat(optimizationOutput, tempStr);
    }

    // ����Ż�����
    strcat(optimizationOutput, "\n�Ż�����:\n");

    // ���ĳ�����ʹ���ʸߣ���������
    for (int i = 0; i < 3 && i < shelfCount; i++) {
        if (utilizations[i] > 0.8) {  // ʹ���ʳ���80%
            Shelf* shelf = findShelfById(shelfIds[i]);
            char typeStr[20];

            switch (shelf->type) {
            case 0: strcpy(typeStr, "��С����"); break;
            case 1: strcpy(typeStr, "С����"); break;
            case 2: strcpy(typeStr, "�а���"); break;
            case 3: strcpy(typeStr, "�����"); break;
            case 4: strcpy(typeStr, "�������"); break;
            case 5: strcpy(typeStr, "����Ʒ"); break;
            case 6: strcpy(typeStr, "����"); break;
            default: strcpy(typeStr, "δ֪");
            }

            char tempStr[100];
            sprintf(tempStr, "- ��������%s���ͻ��ܵ�����\n", typeStr);
            strcat(optimizationOutput, tempStr);
        }
    }

    // ���ĳ�����ʹ���ʵͣ��������
    for (int i = shelfCount - 1; i >= shelfCount - 3 && i >= 0; i--) {
        if (utilizations[i] < 0.2) {  // ʹ���ʵ���20%
            Shelf* shelf = findShelfById(shelfIds[i]);
            char typeStr[20];

            switch (shelf->type) {
            case 0: strcpy(typeStr, "��С����"); break;
            case 1: strcpy(typeStr, "С����"); break;
            case 2: strcpy(typeStr, "�а���"); break;
            case 3: strcpy(typeStr, "�����"); break;
            case 4: strcpy(typeStr, "�������"); break;
            case 5: strcpy(typeStr, "����Ʒ"); break;
            case 6: strcpy(typeStr, "����"); break;
            default: strcpy(typeStr, "δ֪");
            }

            char tempStr[100];
            sprintf(tempStr, "- ���Լ���%s���ͻ��ܵ�����\n", typeStr);
            strcat(optimizationOutput, tempStr);
        }
    }

    // �ͷ��ڴ�
    free(utilizations);
    free(shelfIds);
}