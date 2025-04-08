#ifndef UTIL_H
#define UTIL_H

#include "main.h"

// 隐藏光标的 ANSI 转义序列
#define HIDE_CURSOR "\e[?25l"
// 显示光标的 ANSI 转义序列
#define SHOW_CURSOR "\e[?25h"

void saveAllData();

void loadAllData();

void getCurrentTimeString(char* timeStr);

int generateUniqueId();


int isValidDateFormat(const char* dateStr);

void gettimeonlyday(char *str);

char* timeinput();

void timecheck();

int getDailyIncrementalNumber();//根据每天时间生成逐渐增大的数，函数在当天的第一次调用输出1，第二次调用输出2，在第二天重新从1开始输出

char* getNexttime(const char* dateStr,int time);

void initializedateFile();

void set_cursor_visibility(int visible);

void loading_simulation();

int isValidMonthFormat(const char* month);

void linearRegression(int n, double x[], double y[], double* a, double* b);

int dataprepocessing(Record records[]);

int daysummary(Record records[], int recordCount, DailySummary summaries[]);
#endif /* UTIL_H */
