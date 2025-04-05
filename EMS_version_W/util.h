#ifndef UTIL_H
#define UTIL_H

#include "main.h"

// 获取当前时间字符串
void getCurrentTimeString(char* timeStr);

// 生成唯一ID
int generateUniqueId();

// 检查日期格式是否有效
int isValidDateFormat(const char* dateStr);

void gettimeonlyday(char *str);

char* timeinput();

void timecheck();

//unsigned int hashString(const void* key, size_t len);

int getDailyIncrementalNumber();//根据每天时间生成逐渐增大的数，函数在当天的第一次调用输出1，第二次调用输出2，在第二天重新从1开始输出

char* getNextDay(const char* dateStr);

void initializedateFile();

#endif /* UTIL_H */
