#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "auth.h"
#include "user.h"

// ��¼��Ϣ
int g_currentUserId = -1;    // -1δ��¼
int g_currentUserType = -1;  // �û����ͣ�0��ͨ�û���1����Ա

// �û���¼
int login(const char* username, const char* password) {
	// ����Ƿ��Ѿ���¼
	if (g_currentUserId != -1) {
		return -2; // �Ѿ���¼
	}

	// ���Թ���Ա��¼
	if (adminLogin(username, password)) {
		return 1; // ����Ա��¼�ɹ�
	}

	// ������ͨ�û���¼
	int userId = authenticateUser(username, password);
	if (userId > 0) {
		g_currentUserId = userId;
		g_currentUserType = USER_TYPE_NORMAL; // ��ͨ�û�
		return 2; // ��ͨ�û���¼�ɹ�
	}
	else if (userId == -1) {
		return -1; // �û�������
	}
	else {
		return 0; // �������
	}
}

// ����Ա��¼
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

// �û���֤
int authenticateUser(const char* username, const char* password) {
	User* user = findUserByUsername(username);

	if (user == NULL) {
		return -1; // �û�������
	}

	if (strcmp(user->password, password) != 0) {
		return 0; // �������
	}

	return user->id; // �����û�ID
}

// �ǳ�
void logout() {
	g_currentUserId = -1;
	g_currentUserType = -1;
}

// ��鵱ǰ�Ƿ��ѵ�¼
int isLoggedIn() {
	return g_currentUserId != -1;
}

// ��鵱ǰ�Ƿ��Թ���Ա��ݵ�¼
int isAdminLoggedIn() {
	return g_currentUserType == USER_TYPE_ADMIN;
}

void changePassword() {
	char oldPassword[50];
	char newPassword[50];
	char confirmPassword[50];
	User* currentUser = findUserById(g_currentUserId);
	printf("�����������: ");
	scanf("%s", oldPassword);
	if (strcmp(oldPassword, currentUser->password) != 0) {
		printf("���������\n");
		waitForKeyPress();
		return;
	}
	printf("������������: ");
	scanf("%s", newPassword);
	printf("���ٴ�����������: ");
	scanf("%s", confirmPassword);
	if (strcmp(newPassword, confirmPassword) != 0) {
		printf("������������벻һ�£�\n");
		waitForKeyPress();
		return;
	}
	strcpy(currentUser->password, newPassword);
	printf("�����޸ĳɹ���\n");
	saveUsersToFile("users.txt");
	waitForKeyPress();
}