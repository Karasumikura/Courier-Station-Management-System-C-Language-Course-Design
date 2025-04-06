#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "auth.h"
#include "user.h"

//登陆系统
int g_currentUserId = -1;    
int g_currentUserType = -1;  

int login(const char* username, const char* password) {
	
	if (g_currentUserId != -1) {
		return -2; 
	}

	
	if (adminLogin(username, password)) {
		return 1; 
	}

	
	int userId = authenticateUser(username, password);
	if (userId > 0) {
		g_currentUserId = userId;
		g_currentUserType = USER_TYPE_NORMAL;
		return 2;
	}
	else if (userId == -1) {
		return -1; 
	}
	else {
		return 0; 
	}
}


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


int authenticateUser(const char* username, const char* password) {
	User* user = findUserByUsername(username);

	if (user == NULL) {
		return -1; 
	}

	if (strcmp(user->password, password) != 0) {
		return 0; 
	}

	return user->id; 
}



void logout() {
	g_currentUserId = -1;
	g_currentUserType = -1;
}


int isLoggedIn() {
	return g_currentUserId != -1;
}

 
int isAdminLoggedIn() {
	return g_currentUserType == USER_TYPE_ADMIN;
}

void changePassword() {
	char oldPassword[50];
	char newPassword[50];
	char confirmPassword[50];
	User* currentUser = findUserById(g_currentUserId);
	printf("请输入旧密码: ");
	scanf("%s", oldPassword);
	if (strcmp(oldPassword, currentUser->password) != 0) {
		printf("旧密码错误！\n");
		waitForKeyPress();
		return;
	}
	printf("请输入新密码: ");
	scanf("%s", newPassword);
	printf("请再次输入新密码: ");
	scanf("%s", confirmPassword);
	if (strcmp(newPassword, confirmPassword) != 0) {
		printf("两次输入的密码不一致！\n");
		waitForKeyPress();
		return;
	}
	strcpy(currentUser->password, newPassword);
	printf("密码修改成功！\n");
	saveUsersToFile("users.txt");
	waitForKeyPress();
}