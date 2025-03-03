#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "auth.h"
#include "user.h"

// 全局当前登录用户信息
int g_currentUserId = -1;    // -1表示未登录
int g_currentUserType = -1;  // 用户类型：0普通用户，1管理员

// 用户登录
int login(const char* username, const char* password) {
	// 检查是否已经登录
	if (g_currentUserId != -1) {
		return -2; // 已经登录
	}

	// 尝试管理员登录
	if (adminLogin(username, password)) {
		return 1; // 管理员登录成功
	}

	// 尝试普通用户登录
	int userId = authenticateUser(username, password);
	if (userId > 0) {
		g_currentUserId = userId;
		g_currentUserType = USER_TYPE_NORMAL; // 普通用户
		return 2; // 普通用户登录成功
	}
	else if (userId == -1) {
		return -1; // 用户不存在
	}
	else {
		return 0; // 密码错误
	}
}

// 管理员登录
int adminLogin(const char* username, const char* password) {
	User* user = findUserByUsername(username);

	if (user != NULL && user->memberLevel == USER_ADMIN &&
		strcmp(user->password, password) == 0) {
		g_currentUserId = user->id;
		g_currentUserType = USER_TYPE_ADMIN;
		return 1;
	}

	return 0;
}

// 用户认证
int authenticateUser(const char* username, const char* password) {
	User* user = findUserByUsername(username);

	if (user == NULL) {
		return -1; // 用户不存在
	}

	if (strcmp(user->password, password) != 0) {
		return 0; // 密码错误
	}

	return user->id; // 返回用户ID
}

// 登出
void logout() {
	g_currentUserId = -1;
	g_currentUserType = -1;
}

// 检查当前是否已登录
int isLoggedIn() {
	return g_currentUserId != -1;
}

// 检查当前是否以管理员身份登录
int isAdminLoggedIn() {
	return g_currentUserType == USER_TYPE_ADMIN;
}