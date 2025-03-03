#ifndef UTIL_H
#define UTIL_H

#include "main.h"

// 获取当前时间字符串
void getCurrentTimeString(char* timeStr);

// 生成唯一ID
int generateUniqueId();

// 检查日期格式是否有效
int isValidDateFormat(const char* dateStr);

// 比较两个日期字符串
int compareDates(const char* date1, const char* date2);

// 计算两个日期之间的天数
int daysBetweenDates(const char* date1, const char* date2);

// 将整数转换为字符串
void intToString(int value, char* str);

// 将双精度浮点数转换为字符串
void doubleToString(double value, char* str);

// 计算字符串的哈希值
unsigned int hashString(const char* str);

// 获取下一个工作日
void getNextWorkDay(const char* date, char* nextWorkDay);

#endif /* UTIL_H */#pragma once
