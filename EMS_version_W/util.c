#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "main.h"
#include "util.h"

// 生成唯一ID
int generateUniqueId() {
    // 使用随机数和时间混合生成
    srand((unsigned int)time(NULL) + rand());
    return rand() % 9000 + 1000; // 1000-9999之间的ID
}

// 获取当前时间字符串
void getCurrentTimeString(char* timeStr) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);

    sprintf(timeStr, "%04d-%02d-%02d %02d:%02d:%02d",
        tm_info->tm_year + 1900, tm_info->tm_mon + 1, tm_info->tm_mday,
        tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
}

// 字符串哈希函数
/*unsigned int hashString(const void* key, size_t len) {
    const unsigned char* str = (const unsigned char*)key;
    unsigned int hash = 5381;

    for (size_t i = 0; i < len; i++) {
        hash = ((hash << 5) + hash) + str[i];
    }

    return hash;
}*/