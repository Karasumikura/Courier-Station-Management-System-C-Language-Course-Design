#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "main.h"
#include "util.h"
#include "user.h"
int Promotionstatus;
static int lastDay = -1; // 初始化为无效值
static int counter = 0;
void timecheck() {
    struct tm start_time = { 0 }, end_time = { 0 };
    User* currentUser = findUserById(g_currentUserId);
    start_time.tm_year = 2025 - 1900;
    start_time.tm_mon = 2;//从月份0开始！
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
// 生成唯一ID
int generateUniqueId() {
    // 使用随机数和时间混合生成
    srand((unsigned int)time(NULL) + rand());
    return rand() % 9000 + 1000; // 1000-9999之间的ID
}

// 获取当前时间字符串
void getCurrentTimeString(char* timeStr) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);

    sprintf(timeStr, "%04d-%02d-%02d %02d:%02d:%02d",
        tm_info->tm_year + 1900, tm_info->tm_mon + 1, tm_info->tm_mday,
        tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
}

// 字符串哈希函数
/*unsigned int hashString(const void* key, size_t len) {
    const unsigned char* str = (const unsigned char*)key;
    unsigned int hash = 5381;

    for (size_t i = 0; i < len; i++) {
        hash = ((hash << 5) + hash) + str[i];
    }

    return hash;暂时不使用，备用
}*/

char* timeinput() {
    char timeStr[40];
    getchar();
    printf("请输入时间（格式：YYYY-MM-DD HH:MM:SS）：");
    if (fgets(timeStr, sizeof(timeStr), stdin) == NULL) {
        printf("读取输入失败！\n");
        waitForKeyPress();
        return NULL;
    }

    // 去掉换行符
    size_t len = strlen(timeStr);
    if (len > 0 && timeStr[len - 1] == '\n') {
        timeStr[len - 1] = '\0';
    }

    // 检查格式
    if (!isValidDateFormat(timeStr)) {
        printf("日期格式无效！\n");
        waitForKeyPress();
        return NULL;
    }
    return _strdup(timeStr);
}

int isValidDateFormat(const char* dateStr) {
    int year, month, day, hour, minute, second;
    if (sscanf(dateStr, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) != 6) {
        return 0; // 解析失败
    }

    // 验证日期范围
    struct tm tm_date = { 0 };
    tm_date.tm_year = year - 1900;
    tm_date.tm_mon = month - 1;
    tm_date.tm_mday = day;
    tm_date.tm_hour = hour;
    tm_date.tm_min = minute;
    tm_date.tm_sec = second;

    // 使用 mktime 验证日期有效性
    if (mktime(&tm_date) == -1) {
        return 0; // 日期无效
    }

    return 1;
}

int getDailyIncrementalNumber() {
    // 获取当前时间
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);

    int currentDay = tm_info->tm_yday; // tm_yday 是一年中的第几天（0-365）

    // 判断是否是新的一天
    if (currentDay != lastDay) {
        lastDay = currentDay;
        counter = 1;
    }
    else {
        counter++;
    }

    return counter;
}

char* getNextDay(const char* dateStr) {
    struct tm tm_date = { 0 };
    time_t raw_time;

    // 解析输入日期
    if (sscanf(dateStr, "%d-%d-%d", &tm_date.tm_year, &tm_date.tm_mon, &tm_date.tm_mday) != 3) {
        return NULL; // 解析失败
    }

    // 调整年份和月份的偏移
    tm_date.tm_year -= 1900;
    tm_date.tm_mon -= 1;

    // 转换为时间戳并增加一天
    raw_time = mktime(&tm_date);
    if (raw_time == -1) {
        return NULL; // 转换失败
    }
    raw_time += 24 * 60 * 60; // 增加一天（秒数）

    // 将时间戳转换回日期
    struct tm result;
    if (_localtime64_s(&result, (__time64_t*)&raw_time) != 0) {
        return NULL; // 转换失败
    }

    // 格式化为字符串
    char* nextDay = (char*)malloc(20 * sizeof(char));
    if (nextDay == NULL) {
        return NULL;
    }
    sprintf(nextDay, "%04d-%02d-%02d",
        result.tm_year + 1900,
        result.tm_mon + 1,
        result.tm_mday);

    return nextDay;
}