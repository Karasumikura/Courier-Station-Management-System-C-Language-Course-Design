#ifndef AUTH_H
#define AUTH_H

#include "main.h"

// 用户登录
int login(const char* username, const char* password);

// 管理员登录
int adminLogin(const char* username, const char* password);

// 用户认证
int authenticateUser(const char* username, const char* password);

// 修改密码
int changePassword(int userId, const char* oldPassword, const char* newPassword);



// 当前登录状态检查
int isLoggedIn();

// 当前是否为管理员
int isAdminLoggedIn();

// 登出
void logout();

#endif /* AUTH_H */
