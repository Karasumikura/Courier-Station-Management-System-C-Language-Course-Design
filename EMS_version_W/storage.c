#include <stdio.h>
#include "main.h"
#include "storage.h"
#include "user.h"
#include "package.h"
#include "shelf.h"
#include "transaction.h"

// ��ʼ�����ݴ洢
void initDataStorage() {
	// ȷ������Ŀ¼����
	// ʵ��Ӧ���п�����Ҫ����Ŀ¼
}

// ������������
void saveAllData() {
	saveUsersToFile("users.txt");
	savePackagesToFile("packages.txt");
	saveShelvesToFile("shelves.txt");
	saveTransactionsToFile("transactions.txt");
}

// ������������
void loadAllData() {
	loadUsersFromFile("users.txt");
	loadPackagesFromFile("packages.txt");
	loadShelvesFromFile("shelves.txt");
	loadTransactionsFromFile("transactions.txt");
}