#ifndef UTIL_H
#define UTIL_H

#include "main.h"

// ���ع��� ANSI ת������
#define HIDE_CURSOR "\e[?25l"
// ��ʾ���� ANSI ת������
#define SHOW_CURSOR "\e[?25h"

void saveAllData();

void loadAllData();

void getCurrentTimeString(char* timeStr);

int generateUniqueId();


int isValidDateFormat(const char* dateStr);

void gettimeonlyday(char *str);

char* timeinput();

void timecheck();

int getDailyIncrementalNumber();//����ÿ��ʱ����������������������ڵ���ĵ�һ�ε������1���ڶ��ε������2���ڵڶ������´�1��ʼ���

char* getNexttime(const char* dateStr,int time);

void initializedateFile();

void set_cursor_visibility(int visible);

void loading_simulation();

int isValidMonthFormat(const char* month);

void linearRegression(int n, double x[], double y[], double* a, double* b);

int dataprepocessing(Record records[]);

int daysummary(Record records[], int recordCount, DailySummary summaries[]);
#endif /* UTIL_H */
