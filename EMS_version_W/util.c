#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include "main.h"
#include "util.h"
#include "user.h"
int Promotionstatus;
static int lastDay = -1; 
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

int generateUniqueId() {
    
    srand((unsigned int)time(NULL) + rand());
    return rand() % 9000 + 1000; 
}

void getCurrentTimeString(char* timeStr) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);

    sprintf(timeStr, "%04d-%02d-%02d %02d:%02d:%02d",
        tm_info->tm_year + 1900, tm_info->tm_mon + 1, tm_info->tm_mday,
        tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
}
//strftime 是一个用于格式化日期和时间的函数。
// 它根据指定的格式字符串将 struct tm 类型的时间信息转换为字符串形式
void gettimeonlyday(char *dateStr) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(dateStr, 11, "%Y-%m-%d", tm_info);
}

char* timeinput() {
    char timeStr[40];
    getchar();
    printf("请输入时间（格式：YYYY-MM-DD HH:MM:SS）：");
    if (fgets(timeStr, sizeof(timeStr), stdin) == NULL) {
        printf("读取输入失败！\n");
        waitForKeyPress();
        return NULL;
    }


    size_t len = strlen(timeStr);
    if (len > 0 && timeStr[len - 1] == '\n') {
        timeStr[len - 1] = '\0';
    }


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
        return 0; 
    }

   
    struct tm tm_date = { 0 };
    tm_date.tm_year = year - 1900;
    tm_date.tm_mon = month - 1;
    tm_date.tm_mday = day;
    tm_date.tm_hour = hour;
    tm_date.tm_min = minute;
    tm_date.tm_sec = second;

   
    if (mktime(&tm_date) == -1) {
        return 0; 
    }

    return 1;
}

char* getNextDay(const char* dateStr) {
    struct tm tm_date = { 0 };
    time_t raw_time;

    
    if (sscanf(dateStr, "%d-%d-%d", &tm_date.tm_year, &tm_date.tm_mon, &tm_date.tm_mday) != 3) {
        return NULL; 
    }

    
    tm_date.tm_year -= 1900;
    tm_date.tm_mon -= 1;

    
    raw_time = mktime(&tm_date);
    if (raw_time == -1) {
        return NULL; 
    }
    raw_time += 24 * 60 * 60; 

    
    struct tm result;
    if (_localtime64_s(&result, (__time64_t*)&raw_time) != 0) {
        return NULL; 
    }

    
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

void initializedateFile() {
    FILE* file = fopen("counter_data.txt", "w");
    if (file == NULL) {
        printf("无法创建文件！\n");
        exit(1);
    }
    char currentDate[11];
    gettimeonlyday(currentDate);
    fprintf(file, "%s\n1\n", currentDate); 
    fclose(file);
}

int readCounterData(char* lastDate, int* counter) {
    FILE* file = fopen("counter_data.txt", "r");
    if (file == NULL) {
        return 0; 
    }

    
    if (fscanf(file, "%10s\n%d", lastDate, counter) != 2) {
        fclose(file);
        return 0; 
    }

    fclose(file);
    return 1; 
}


void writeCounterData(const char* currentDate, int counter) {
    FILE* file = fopen("counter_data.txt", "w");
    if (file == NULL) {
        printf("无法写入文件！\n");
        exit(1);
    }
    fprintf(file, "%s\n%d\n", currentDate, counter); 
    fclose(file);
}


int getDailyIncrementalNumber() {
    char currentDate[11];
    char lastDate[11];
    int counter;

   
    gettimeonlyday(currentDate);

    
    if (!readCounterData(lastDate, &counter)) {
        // 如果文件不存在或读取失败，那么就初始化文件
        initializedateFile();
        strcpy(lastDate, currentDate);
        counter = 1;
    }
    else {
        
        if (strcmp(currentDate, lastDate) != 0) {
          
            counter = 1;
        }
        else {
           
            counter++;
        }
    }

    
    writeCounterData(currentDate, counter);

    return counter;
}

void set_cursor_visibility(int visible) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);  // 获取当前光标信息
    cursorInfo.bVisible = visible;               // 设置光标是否可见
    SetConsoleCursorInfo(hConsole, &cursorInfo); // 应用新的光标信息
}

void loading_simulation() {
    set_cursor_visibility(0);
    for (int i = 0; i < WAITING_TIME; i++) {
        switch (i % 3) {
        case 0:
            printf("\r加载中.  ");  // \r 回到行首
            break;
        case 1:
            printf("\r加载中.. ");
            break;
        case 2:
            printf("\r加载中...");
            break;
        }
        fflush(stdout);  // 确保立即输出到控制台
        Sleep(100);
    }
    printf("\r       \n");
    set_cursor_visibility(1);
}