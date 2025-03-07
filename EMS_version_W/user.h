#ifndef USER_H
#define USER_H

#include "main.h"

// ��ʼ���û�����
void initUserList();

// ����û�
User* addUser(const char* username, const char*phonenumber , const char* password, int memberLevel);

//��麯��
void AddUserCheck();

// �����û���ͨ���û�����
User* findUserByUsername(const char* username);

// �����û���ͨ��ID��
User* findUserById(int userId);

// �����û�ByPhone
User* findUserByPhone(const char* phonenumber);


// ɾ���û�
int deleteUser(int userId);


// �����û����ѵȼ�
void updateUserConsumptionLevel(int userId, double packageValue);

// �����û���Ա�ȼ�
void upgradeUserMemberLevel(int userId);

// �����û����ݵ��ļ�
void saveUsersToFile(const char* filename);

// ���ļ������û�����
void loadUsersFromFile(const char* filename);

// �ͷ��û������ڴ�
//void freeUserList();

#endif /* USER_H */
