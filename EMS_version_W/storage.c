#include <stdio.h>
#include "main.h"
#include "storage.h"
#include "user.h"
#include "package.h"
#include "shelf.h"
#include "transaction.h"
#include "ml_analysis.h"

void initDataStorage() {
	// 确保数据目录存在
	// 实际应用中可能需要创建目录，开发中
}

void saveAllData() {
	saveUsersToFile("users.txt");
	savePackages_File("packages.txt");
	saveShelvesToFile("shelves.txt");
	saveTransactionsToFile("transactions.txt");
	save_model(&shelf_model, "shelf_model.bin");
}

void loadAllData() {
	loadUsersFromFile("users.txt");
	loadPackagesFromFile("packages.txt");
	loadShelvesFromFile("shelves.txt");
	loadTransactionsFromFile("transactions.txt");
	load_model(&shelf_model, "shelf_model.bin");
}

