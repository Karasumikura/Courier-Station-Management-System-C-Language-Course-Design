#ifndef USER_H
#define USER_H

#include "main.h"

// ��ʼ���û�����
void initUserList();

// ����û�
User* addUser(const char* username, const char*phonenumber , const char* password, int memberLevel);

// �����û���ͨ���û�����
User* findUserByUsername(const char* username);

// �����û���ͨ��ID��
User* findUserById(int userId);

// �����û�ByPhone
User* findUserByPhone(const char* phonenumber);

// �����û���Ϣ
int updateUser(int userId, const char* password, int memberLevel, double consumptionLevel);

// ɾ���û�
int deleteUser(int userId);

// �û������֤
int authenticateUser(const char* username, const char* password);

// �����û����ѵȼ�
void updateUserConsumptionLevel(int userId, double packageValue);

// �����û����ݵ��ļ�
void saveUsersToFile(const char* filename);

// ���ļ������û�����
void loadUsersFromFile(const char* filename);

// �ͷ��û������ڴ�
//void freeUserList();

#endif /* USER_H */
