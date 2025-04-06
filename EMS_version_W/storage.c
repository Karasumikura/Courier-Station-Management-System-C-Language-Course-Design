#include <stdio.h>
#include "main.h"
#include "storage.h"
#include "user.h"
#include "package.h"
#include "shelf.h"
#include "transaction.h"


void initDataStorage() {
	// 确保数据目录存在
	
}


void saveAllData() {
	saveUsersToFile("users.txt");
	savePackages_File("packages.txt");
	saveShelvesToFile("shelves.txt");
	saveTransactionsToFile("transactions.txt");
}

void loadAllData() {
	loadUsersFromFile("users.txt");
	loadPackagesFromFile("packages.txt");
	loadShelvesFromFile("shelves.txt");
	loadTransactionsFromFile("transactions.txt");
}

