#ifndef AUTH_H
#define AUTH_H

#include "main.h"

// �û���¼
int login(const char* username, const char* password);

// ����Ա��¼
int adminLogin(const char* username, const char* password);

// �û�ע��
int registerUser(const char* username, const char* password);

// �޸�����
int changePassword(int userId, const char* oldPassword, const char* newPassword);

// �����������
void generateRandomPassword(char* password, int length);

// ��ǰ��¼״̬���
int isLoggedIn();

// ��ǰ�Ƿ�Ϊ����Ա
int isAdmin();

// �ǳ�
void logout();

#endif /* AUTH_H */
