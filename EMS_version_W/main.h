#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ��������
#define USER_NEW 0       // ���û�
#define USER_SILVER 1    // ������Ա
#define USER_GOLD 2      // �ƽ��Ա
#define USER_ADMIN 3     // ����Ա

#define USER_TYPE_NORMAL 0  // ��ͨ�û�����
#define USER_TYPE_ADMIN 1   // ����Ա����

// ������С����
#define PACKAGE_SIZE_TINY 0
#define PACKAGE_SIZE_SMALL 1
#define PACKAGE_SIZE_MEDIUM 2
#define PACKAGE_SIZE_LARGE 3
#define PACKAGE_SIZE_HUGE 4

// �������Զ���
#define PACKAGE_NOTE_NONE 0
#define PACKAGE_NOTE_FRAGILE 1
#define PACKAGE_NOTE_FRESH 2

// �������䷽ʽ����
#define TRANSPORT_NORMAL 0        // ������������
#define TRANSPORT_FAST_ROAD 1     // �ӿ죨��·��
#define TRANSPORT_EXPRESS_AIR 2   // �ؿ죨���ˣ�
#define TRANSPORT_EXPRESS_ROAD 3  // �ؿ죨��·��

// ����״̬����
#define PACKAGE_STATUS_WAITING 0  // ��ȡ
#define PACKAGE_STATUS_PICKED 1   // ��ȡ
#define PACKAGE_STATUS_ABNORMAL 2 // �쳣

// �������Ͷ���
#define TRANSACTION_INCOME 0      // ����
#define TRANSACTION_EXPENSE 1     // ֧��

// ����������
#define INCOME_PIECE_FEE 0        // �Ƽ���
#define INCOME_STORAGE_FEE 1      // �����

// ֧��������
#define EXPENSE_COMPENSATION 0    // �⳥��
#define EXPENSE_MAINTENANCE 1     // ά����

// �û��ṹ��
typedef struct User {
	int id;                  // �û�ID
	char username[50];       // �û���
	char password[50];       // ����
	char phonenumber[15];    // �绰����
	int memberLevel;         // ��Ա�ȼ�
	double consumptionLevel; // ���������ȼ�
	struct User* next;       // ������һ�ڵ�
} User;

// �����ṹ��
typedef struct Package {
	int id;                  // ����ID
	int userId;              // �����û�ID
	int size;                // ��С
	int weight;              // �����ȼ���0-4
	int note;                // ��ע����
	int transportMethod;     // ���䷽ʽ
	double value;            // �������ֵ
	char pickupCode[20];     // ȡ����
	int shelfId;             // ��Ż���ID
	int status;              // ״̬
	char createTime[20];     // ����ʱ��
	struct Package* next;    // ������һ�ڵ�
} Package;

// ���ܽṹ��
typedef struct Shelf {
	int id;                  // ����ID
	int type;                // ���ͣ����ڴ�С�����ԣ�
	int capacity;            // ������
	int currentCount;        // ��ǰ�������
	struct Shelf* next;      // ������һ�ڵ�
} Shelf;

// ҵ���¼�ṹ��
typedef struct Transaction {
	int id;                  // ��¼ID
	int type;                // ���ͣ�0-���룬1-֧��
	int subType;             // ������
	double amount;           // ���
	char description[100];   // ����
	char createTime[20];     // ����ʱ��
	struct Transaction* next;// ������һ�ڵ�
} Transaction;

// ȫ������ͷ����
extern User* g_userList;
extern Package* g_packageList;
extern Shelf* g_shelfList;
extern Transaction* g_transactionList;

// ��ǰ��¼��Ϣ
extern int g_currentUserId;
extern int g_currentUserType;

// ϵͳ��������
void clearScreen();
void waitForKeyPress();
void initSystem();
void showMainMenu();
void showAdminMenu();
void showUserMenu();

void clearInputBuffer();

#endif /* MAIN_H */