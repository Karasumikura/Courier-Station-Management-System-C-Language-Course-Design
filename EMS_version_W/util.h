#ifndef UTIL_H
#define UTIL_H

#include "main.h"


void getCurrentTimeString(char* timeStr);

int generateUniqueId();


int isValidDateFormat(const char* dateStr);

void gettimeonlyday(char *str);

char* timeinput();

void timecheck();

//unsigned int hashString(const void* key, size_t len);

int getDailyIncrementalNumber();//����ÿ��ʱ����������������������ڵ���ĵ�һ�ε������1���ڶ��ε������2���ڵڶ������´�1��ʼ���

char* getNextDay(const char* dateStr);

void initializedateFile();

#endif /* UTIL_H */
