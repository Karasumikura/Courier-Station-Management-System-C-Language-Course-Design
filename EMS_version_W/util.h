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

//unsigned int hashString(const void* key, size_t len);

int getDailyIncrementalNumber();//根据每天时间生成逐渐增大的数，函数在当天的第一次调用输出1，第二次调用输出2，在第二天重新从1开始输出

#endif /* UTIL_H */
