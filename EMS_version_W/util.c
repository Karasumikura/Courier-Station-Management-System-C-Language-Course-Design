#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>  // Windows 平台使用
#else
#include <unistd.h>   // Linux/Unix 平台使用
#endif
#include "main.h"
#include "util.h"
#include "user.h"
int Promotionstatus;
static int lastDay = -1; 
static int counter = 0;

void saveAllData() {
    saveUsersToFile("users.txt");
    savePackages_File("packages.txt");
    saveShelvesToFile("shelves.txt");
    saveTransactionsToFile("transactions.txt");
}

void loadAllData() {
    loadUsersFromFile("users.txt");
    loadPackagesFromFile("packages.txt");
    loadShelvesFromFile("shelves.txt");
    loadTransactionsFromFile("transactions.txt");
}

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

char* getNexttime(const char* dateStr,int time) {
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
    raw_time += 24 * 60 * 60 * time; 

    
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
#ifdef _WIN32
    // 隐藏光标（Windows 平台）
    set_cursor_visibility(0);
#else
    // 隐藏光标（Linux/Unix 平台）
    printf(HIDE_CURSOR);
    fflush(stdout);
#endif
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
    printf("\r         \r");
#ifdef _WIN32
    // 恢复光标（Windows 平台）
    set_cursor_visibility(1);
#else
    // 恢复光标（Linux/Unix 平台）
    printf(SHOW_CURSOR);
    fflush(stdout);
#endif
}

int isValidMonthFormat(const char* month) {
    // 检查字符串长度是否为 7
    if (strlen(month) != 7) {
        return 0; // 长度不符合要求
    }

    // 检查前 4 个字符是否为数字
    for (int i = 0; i < 4; i++) {
        if (!isdigit(month[i])) {
            return 0; // 年份部分包含非数字字符
        }
    }

    // 检查第 5 个字符是否为 '-'
    if (month[4] != '-') {
        return 0; // 分隔符错误
    }

    for (int i = 5; i < 7; i++) {
        if (!isdigit(month[i])) {
            return 0; // 月份部分包含非数字字符
        }
    }

    // 提取月份并验证范围
    int monthValue = atoi(month + 5); // 提取月份部分
    if (monthValue < 1 || monthValue > 12) {
        return 0; // 月份不在有效范围内
    }

    return 1; // 格式正确
}

void linearRegression(int n, double x[], double y[], double* a, double* b) {
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;

    for (int i = 0; i < n; i++) {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_x2 += x[i] * x[i];
    }

    *a = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
    *b = (sum_y - (*a) * sum_x) / n;
}//用来算线性回归的函数

int dataprepocessing(const char* filename, Record records[]) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("无法打开文件 %s！\n", filename);
        return 0;
    }

    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // 提取时间戳和价格字段
        char* token = strtok(line, ",");
        int fieldIndex = 0;
        while (token != NULL && fieldIndex < 5) {
            if (fieldIndex == 3) { // 第4列为价格
                records[count].price = atof(token);
            }
            else if (fieldIndex == 4) { // 第5列为时间戳
                strncpy(records[count].timestamp, token, 19);
                records[count].timestamp[19] = '\0';
            }
            token = strtok(NULL, ",");
            fieldIndex++;
        }
        count++;
    }
    fclose(file);
    return count;
}