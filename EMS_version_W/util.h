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

// ������ת��Ϊ�ַ���
void intToString(int value, char* str);

// ��˫���ȸ�����ת��Ϊ�ַ���
void doubleToString(double value, char* str);

// �����ַ����Ĺ�ϣֵ
unsigned int hashString(const char* str);

// ��ȡ��һ��������
void getNextWorkDay(const char* date, char* nextWorkDay);

#endif /* UTIL_H */#pragma once
