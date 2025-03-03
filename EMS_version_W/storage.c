#include <stdio.h>
#include "main.h"
#include "storage.h"
#include "user.h"
#include "package.h"
#include "shelf.h"
#include "transaction.h"

// 初始化数据存储
void initDataStorage() {
	// 确保数据目录存在
	// 实际应用中可能需要创建目录
}

// 保存所有数据
void saveAllData() {
	saveUsersToFile("users.txt");
	savePackagesToFile("packages.txt");
	saveShelvesToFile("shelves.txt");
	saveTransactionsToFile("transactions.txt");
}

// 加载所有数据
void loadAllData() {
	loadUsersFromFile("users.txt");
	loadPackagesFromFile("packages.txt");
	loadShelvesFromFile("shelves.txt");
	loadTransactionsFromFile("transactions.txt");
}