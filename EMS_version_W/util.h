#ifndef UTIL_H
#define UTIL_H

#include "main.h"

// ���ع��� ANSI ת������
#define HIDE_CURSOR "\e[?25l"
// ��ʾ���� ANSI ת������
#define SHOW_CURSOR "\e[?25h"


void getCurrentTimeString(char* timeStr);

int generateUniqueId();


int isValidDateFormat(const char* dateStr);

void gettimeonlyday(char *str);

char* timeinput();

void timecheck();

int getDailyIncrementalNumber();//����ÿ��ʱ����������������������ڵ���ĵ�һ�ε������1���ڶ��ε������2���ڵڶ������´�1��ʼ���

char* getNextDay(const char* dateStr);

void initializedateFile();

void set_cursor_visibility(int visible);

void loading_simulation();

#endif /* UTIL_H */
