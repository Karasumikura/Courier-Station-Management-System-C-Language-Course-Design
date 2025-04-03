#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "main.h"
#include "util.h"
#include "user.h"
int Promotionstatus;
static int lastDay = -1; // ��ʼ��Ϊ��Чֵ
static int counter = 0;
void timecheck() {
    struct tm start_time = { 0 }, end_time = { 0 };
    User* currentUser = findUserById(g_currentUserId);
    start_time.tm_year = 2025 - 1900;
    start_time.tm_mon = 2;//���·�0��ʼ��
    start_time.tm_mday = 21;
    start_time.tm_hour = 0;
    start_time.tm_min = 0;
    start_time.tm_sec = 0;

    end_time.tm_year = 2025 - 1900;
    end_time.tm_mon = 5;
    end_time.tm_mday = 21;
    end_time.tm_hour = 23;
    end_time.tm_min = 59;
    end_time.tm_sec = 59;

    time_t start_timestamp = mktime(&start_time);
    time_t end_timestamp = mktime(&end_time);

    time_t current_time = time(NULL);

    if (current_time >= start_timestamp && current_time <= end_timestamp)
    {
        Promotionstatus = 1;
        currentUser->Promotionstatus_user = 1;
    }
    else {
        Promotionstatus = 0;
        currentUser->Promotionstatus_user = 0;
    }

}
// ����ΨһID
int generateUniqueId() {
    // ʹ���������ʱ��������
    srand((unsigned int)time(NULL) + rand());
    return rand() % 9000 + 1000; // 1000-9999֮���ID
}

// ��ȡ��ǰʱ���ַ���
void getCurrentTimeString(char* timeStr) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);

    sprintf(timeStr, "%04d-%02d-%02d %02d:%02d:%02d",
        tm_info->tm_year + 1900, tm_info->tm_mon + 1, tm_info->tm_mday,
        tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
}

// �ַ�����ϣ����
/*unsigned int hashString(const void* key, size_t len) {
    const unsigned char* str = (const unsigned char*)key;
    unsigned int hash = 5381;

    for (size_t i = 0; i < len; i++) {
        hash = ((hash << 5) + hash) + str[i];
    }

    return hash;��ʱ��ʹ�ã�����
}*/

char* timeinput() {
    char* timeStr = (char*)malloc(20 * sizeof(char));
    if (timeStr == NULL) {
        return NULL; // �ڴ����ʧ��
    }
    printf("������ʱ�䣨��ʽ��YYYY-MM-DD HH:MM:SS����");
    scanf("%s", timeStr);
    // ������ڸ�ʽ�Ƿ���Ч
    if (!isValidDateFormat(timeStr)) {
        free(timeStr);
        return NULL; // ���ڸ�ʽ��Ч
    }
    return timeStr;
}

int isValidDateFormat(const char* dateStr) {
    int year, month, day, hour, minute, second;
    if (sscanf(dateStr, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) != 6) {
        return 0;
    }

    // ��֤���ڷ�Χ����ѡ��
    if (year < 1900 || month < 1 || month > 12 || day < 1 || day > 31 ||
        hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
        return 0; 
    }

    return 1; 
}

int getDailyIncrementalNumber() {
    // ��ȡ��ǰʱ��
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);

    int currentDay = tm_info->tm_yday; // tm_yday ��һ���еĵڼ��죨0-365��

    // �ж��Ƿ����µ�һ��
    if (currentDay != lastDay) {
        lastDay = currentDay;
        counter = 1;
    }
    else {
        counter++;
    }

    return counter;
}
