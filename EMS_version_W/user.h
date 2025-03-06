#ifndef USER_H
#define USER_H

#include "main.h"

// 初始化用户链表
void initUserList();

// 添加用户
User* addUser(const char* username, const char*phonenumber , const char* password, int memberLevel);

// 查找用户（通过用户名）
User* findUserByUsername(const char* username);

// 查找用户（通过ID）
User* findUserById(int userId);

// 查找用户ByPhone
User* findUserByPhone(const char* phonenumber);

// 更新用户信息
int updateUser(int userId, const char* password, int memberLevel, double consumptionLevel);

// 删除用户
int deleteUser(int userId);

// 用户身份验证
int authenticateUser(const char* username, const char* password);

// 更新用户消费等级
void updateUserConsumptionLevel(int userId, double packageValue);

// 保存用户数据到文件
void saveUsersToFile(const char* filename);

// 从文件加载用户数据
void loadUsersFromFile(const char* filename);

// 释放用户链表内存
//void freeUserList();

#endif /* USER_H */
