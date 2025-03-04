#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "user.h"
#include "util.h"

// 全局用户链表头
User* g_userList = NULL;

// 初始化用户链表
void initUserList() {
    g_userList = NULL;
}

// 添加用户
User* addUser(const char* username, const char* password, int memberLevel) {
    // 检查用户名是否已存在
    if (findUserByUsername(username) != NULL) {
        return NULL; // 用户名已存在
    }

    // 创建新用户节点
    User* newUser = (User*)malloc(sizeof(User));
    if (newUser == NULL) {
        return NULL; // 内存分配失败
    }

    // 初始化用户数据
    newUser->id = generateUniqueId();
    strncpy(newUser->username, username, sizeof(newUser->username) - 1);
    newUser->username[sizeof(newUser->username) - 1] = '\0';
    strncpy(newUser->password, password, sizeof(newUser->password) - 1);
    newUser->password[sizeof(newUser->password) - 1] = '\0';
    newUser->memberLevel = memberLevel;
    newUser->consumptionLevel = 0.0;
    newUser->next = NULL;

    // 插入到链表头部
    if (g_userList == NULL) {
        g_userList = newUser;
    }
    else {
        newUser->next = g_userList;
        g_userList = newUser;
    }

    return newUser;
}

// 查找用户（通过用户名）
User* findUserByUsername(const char* username) {
    User* current = g_userList;

    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

// 查找用户（通过ID）
User* findUserById(int userId) {
    User* current = g_userList;

    while (current != NULL) {
        if (current->id == userId) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

// 删除用户
int deleteUser(int userId) {
    if (g_userList == NULL) {
        return 0; // 空链表
    }

    // 特殊情况：删除头节点
    if (g_userList->id == userId) {
        User* temp = g_userList;
        g_userList = g_userList->next;
        free(temp);
        return 1;
    }

    // 搜索待删除节点
    User* current = g_userList;
    while (current->next != NULL && current->next->id != userId) {
        current = current->next;
    }

    // 找到节点
    if (current->next != NULL) {
        User* temp = current->next;
        current->next = temp->next;
        free(temp);
        return 1;
    }

    return 0; // 没找到节点
}

// 更新用户消费能力
void updateUserConsumptionLevel(int userId, double packageValue) {
    User* user = findUserById(userId);
    if (user != NULL) {
        // 消费能力值会随着包裹价值逐渐调整
        user->consumptionLevel = (user->consumptionLevel * 0.7) + (packageValue * 0.3);
    }
}

// 升级用户会员等级
void upgradeUserMemberLevel(int userId) {
    User* user = findUserById(userId);
    if (user != NULL) {
        // 根据消费能力自动升级
        if (user->memberLevel == USER_NEW && user->consumptionLevel >= 500) {
            user->memberLevel = USER_SILVER;
        }
        else if (user->memberLevel == USER_SILVER && user->consumptionLevel >= 1000) {
            user->memberLevel = USER_GOLD;
        }
    }
}

// 保存用户数据到文件
void saveUsersToFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法打开用户数据文件 %s\n", filename);
        return;
    }

    User* current = g_userList;
    while (current != NULL) {
        fprintf(file, "%d,%s,%s,%d,%.2f\n",
            current->id,
            current->username,
            current->password,
            current->memberLevel,
            current->consumptionLevel);
        current = current->next;
    }

    fclose(file);
}

// 从文件加载用户数据
void loadUsersFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("用户数据文件 %s 不存在，将创建新文件\n", filename);
        return;
    }

    // 清空现有链表
    while (g_userList != NULL) {
        User* temp = g_userList;
        g_userList = g_userList->next;
        free(temp);
    }

    // 读取文件数据
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        int id;
        char username[50];
        char password[50];
        int memberLevel;
        double consumptionLevel;

        if (sscanf(line, "%d,%[^,],%[^,],%d,%lf",
            &id, username, password, &memberLevel, &consumptionLevel) == 5) {
            // 创建新用户节点
            User* newUser = (User*)malloc(sizeof(User));
            if (newUser != NULL) {
                newUser->id = id;
                strncpy(newUser->username, username, sizeof(newUser->username) - 1);
                newUser->username[sizeof(newUser->username) - 1] = '\0';
                strncpy(newUser->password, password, sizeof(newUser->password) - 1);
                newUser->password[sizeof(newUser->password) - 1] = '\0';
                newUser->memberLevel = memberLevel;
                newUser->consumptionLevel = consumptionLevel;

                // 插入到链表头部
                newUser->next = g_userList;
                g_userList = newUser;
            }
        }
    }

    fclose(file);
}