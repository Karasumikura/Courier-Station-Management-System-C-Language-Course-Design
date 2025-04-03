#ifndef UTIL_H
#define UTIL_H

#include "main.h"

// ��ȡ��ǰʱ���ַ���
void getCurrentTimeString(char* timeStr);

// ����ΨһID
int generateUniqueId();

// ������ڸ�ʽ�Ƿ���Ч
int isValidDateFormat(const char* dateStr);

// �Ƚ����������ַ���
int compareDates(const char* date1, const char* date2);

// ������������֮�������
int daysBetweenDates(const char* date1, const char* date2);

char* timeinput();

void timecheck();


#endif /* UTIL_H */
