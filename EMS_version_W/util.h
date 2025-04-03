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

char* timeinput();

void timecheck();


#endif /* UTIL_H */
