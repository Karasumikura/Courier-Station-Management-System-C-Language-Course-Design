#ifndef AUTH_H
#define AUTH_H

#include "main.h"

// �û���¼
int login(const char* username, const char* password);

// ����Ա��¼
int adminLogin(const char* username, const char* password);

// �û���֤
int authenticateUser(const char* username, const char* password);

// �޸�����
void changePassword();



// ��ǰ��¼״̬���
int isLoggedIn();

// ��ǰ�Ƿ�Ϊ����Ա
int isAdminLoggedIn();

// �ǳ�
void logout();

#endif /* AUTH_H */
