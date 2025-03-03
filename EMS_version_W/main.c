#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "user.h"
#include "package.h"
#include "shelf.h"
#include "transaction.h"
#include "auth.h"
#include "statistics.h"
#include "pricing.h"
#include "storage.h"
#include "util.h"

// 临时声明
int markPackageAsPickedUp(int packageId);
int markPackageAsAbnormal(int packageId, const char* reason);
double calculatePackageFee(int size, int weight, int transportMethod);

// 系统操作函数
void clearScreen();
void waitForKeyPress();
void initSystem();
void showMainMenu();
void showAdminMenu();
void showUserMenu();

// 登录和用户系统函数
void handleRegister();
void handleLogin();

// 用户管理函数
void handleUserManagement();
void displayAllUsers();
void handleAddUser();
void handleEditUser();
void handleDeleteUser();

// 包裹管理函数
void handlePackageManagement();
void displayAllPackages();
void handleAddPackage();
void handleEditPackage();
void handleMarkPackagePickedUp();
void handleMarkPackageAbnormal();
void handleSearchPackage();

// 货架管理函数
void displayAllShelves();
void handleAddShelf();
void handleShelfManagement();

// 清屏函数
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// 等待用户按任意键继续
void waitForKeyPress() {
    printf("\n按任意键继续...");
    getchar();
    getchar(); //清输入区
}

// 初始化系统
void initSystem() {
    // 初始化各种链表
    initUserList();
    initPackageList();
    initShelfList();
    initTransactionList();

    // 加载数据
    loadAllData();

    // 如果货架为空，创建默认货架
    if (g_shelfList == NULL) {
        // 创建5种尺寸的常规货架
        for (int i = 0; i < 5; i++) {
            addShelf(i, 20); // 每种尺寸20个容量
        }

        // 创建特殊货架：易碎品
        addShelf(5, 10); // 易碎品货架，容量10

        // 创建特殊货架：冷鲜
        addShelf(6, 10); // 冷鲜货架，容量10
    }

    // 如果没有管理员账户，创建默认管理员
    if (findUserByUsername("admin") == NULL) {
        addUser("admin", "123456", USER_ADMIN);
    }
}




// 显示主菜单
void showMainMenu() {
    clearScreen();
    printf("=================================\n");
    printf("    欢迎使用快递驿站管理系统    \n");
    printf("=================================\n");
    printf("1. 登录\n");
    printf("2. 注册新用户\n");
    printf("0. 退出系统\n");
    printf("请选择操作：");
}

// 显示管理员菜单
void showAdminMenu() {
    int choice;
    clearScreen();
    printf("=================================\n");
    printf("    快递驿站管理系统 - 管理员    \n");
    printf("=================================\n");
    printf("1. 用户管理\n");
    printf("2. 包裹管理\n");
    printf("3. 库存与货架管理\n");
    printf("4. 数据分析与报表\n");
    printf("5. 交易记录\n");
    printf("0. 登出\n");
    printf("请选择操作：");
    scanf("%d", &choice);
	switch (choice) {
	case 1:
		handleUserManagement();
		break;
	case 2:
		handlePackageManagement();
		break;
	case 3:
		handleShelfManagement();
		break;
	case 4:
		//handleStatistics();
		break;
	case 5:
		//handleTransactions();
		break;
	case 0:
		logout();
		break;
	default:
		printf("无效选择，请重新输入！\n");
		waitForKeyPress();
	}
}

// 显示用户菜单
void showUserMenu() {
    int choice;
    clearScreen();
    User* currentUser = findUserById(g_currentUserId);

    printf("=================================\n");
    printf("    快递驿站管理系统 - 用户    \n");
    printf("=================================\n");
    printf("当前用户：%s (", currentUser->username);

    // 显示会员等级
    switch (currentUser->memberLevel) {
    case USER_NEW:
        printf("新用户");
        break;
    case USER_SILVER:
        printf("白银会员");
        break;
    case USER_GOLD:
        printf("黄金会员");
        break;
    }
    printf(")\n\n");

    printf("1. 查看我的包裹\n");
    printf("2. 取件\n");
    printf("3. 寄件\n");
    printf("4. 查看我的会员信息\n");
    printf("5. 账户设置\n");
    printf("0. 登出\n");
    printf("请选择操作：");
	scanf("%d", &choice);
    switch (choice) {
    case 1:
        //displayMyPackages();
        break;
    case 0:
        logout();
        break;
    default:
        printf("无效选择，请重新输入！\n");
        waitForKeyPress();
    }
}

// 处理登录
    
    void handleLogin() {
        clearScreen();
        printf("=================================\n");
        printf("             登录               \n");
        printf("=================================\n");

        char username[50];
        char password[50];

        printf("账户: ");
        scanf("%s", username);
        printf("密码: ");
        scanf("%s", password);

        int result = login(username, password);

        if (result == 1) {
            printf("管理员登录成功！\n");
            waitForKeyPress();
            showAdminMenu();
        }
        else if (result == 2) {
            printf("用户登录成功！\n");
            waitForKeyPress();
            showUserMenu();
        }
        else if (result == -1) {
            printf("账户不存在！\n");
            waitForKeyPress();
        }
        else if (result == 0) {
            printf("密码错误！\n");
            waitForKeyPress();
        }
        else {
            printf("登录失败！\n");
            waitForKeyPress();
        }
    }



// 处理注册
void handleRegister() {
    clearScreen();
    printf("=================================\n");
    printf("             注册               \n");
    printf("=================================\n");

    char username[50];
    char password[50];
    char confirmPassword[50];

    printf("用户名: ");
    scanf("%s", username);

    // 检查用户名是否存在
    if (findUserByUsername(username) != NULL) {
        printf("该用户名已存在！\n");
        waitForKeyPress();
        return;
    }

    printf("密码: ");
    scanf("%s", password);
    printf("确认密码: ");
    scanf("%s", confirmPassword);

    if (strcmp(password, confirmPassword) != 0) {
        printf("两次输入的密码不一致！\n");
        waitForKeyPress();
        return;
    }

    // 创建新用户
    User* newUser = addUser(username, password, USER_NEW);
    if (newUser == NULL) {
        printf("注册失败！\n");
    }
    else {
        printf("注册成功！您可以使用新账户登录了。\n");
        saveUsersToFile("users.txt");
    }

    waitForKeyPress();
}



// 用户管理
void handleUserManagement() {
    int running = 1;
    while (running) {
        clearScreen();
        printf("=================================\n");
        printf("           用户管理             \n");
        printf("=================================\n");
        printf("1. 查看所有用户\n");
        printf("2. 添加用户\n");
        printf("3. 编辑用户\n");
        printf("4. 删除用户\n");
        printf("0. 返回\n");
        printf("请选择操作：");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            displayAllUsers();
            waitForKeyPress();
            break;
        case 2:
            handleAddUser();
            break;
        case 3:
            handleEditUser();
            break;
        case 4:
            handleDeleteUser();
            break;
        case 0:
            showAdminMenu();
            break;
        default:
            printf("无效选择，请重新输入！\n");
            waitForKeyPress();
        }
    }
}

// 显示所有用户
void displayAllUsers() {
    clearScreen();
    printf("=================================\n");
    printf("           所有用户             \n");
    printf("=================================\n");
    printf("%-5s %-20s %-15s %-15s\n", "ID", "用户名", "会员等级", "消费能力");
    printf("---------------------------------\n");

    User* current = g_userList;
    int count = 0;

    while (current != NULL) {
        // 不显示管理员账户
        if (current->memberLevel != USER_ADMIN) {
            printf("%-5d %-20s ", current->id, current->username);

            // 会员等级
            switch (current->memberLevel) {
            case USER_NEW:
                printf("%-15s ", "新用户");
                break;
            case USER_SILVER:
                printf("%-15s ", "白银会员");
                break;
            case USER_GOLD:
                printf("%-15s ", "黄金会员");
                break;
            default:
                printf("%-15s ", "未知");
            }

            printf("%-15.2f\n", current->consumptionLevel);
            count++;
        }

        current = current->next;
    }

    if (count == 0) {
        printf("暂无普通用户记录\n");
    }
}

// 处理添加用户
void handleAddUser() {
    clearScreen();
    printf("=================================\n");
    printf("           添加用户             \n");
    printf("=================================\n");

    char username[50];
    char password[50];
    int memberLevel;

    printf("用户名: ");
    scanf("%s", username);

    // 检查用户名是否存在
    if (findUserByUsername(username) != NULL) {
        printf("该用户名已存在！\n");
        waitForKeyPress();
        return;
    }

    printf("密码: ");
    scanf("%s", password);

    printf("会员等级 (0-新用户, 1-白银会员, 2-黄金会员): ");
    scanf("%d", &memberLevel);

    if (memberLevel < 0 || memberLevel > 2) {
        printf("无效的会员等级！\n");
        waitForKeyPress();
        return;
    }

    // 创建新用户
    User* newUser = addUser(username, password, memberLevel);
    if (newUser == NULL) {
        printf("添加用户失败！\n");
    }
    else {
        printf("添加用户成功！\n");
        saveUsersToFile("users.txt");
    }

    waitForKeyPress();
}

// 处理编辑用户
void handleEditUser() {
    clearScreen();
    printf("=================================\n");
    printf("           编辑用户             \n");
    printf("=================================\n");

    int userId;
    printf("请输入要编辑的用户ID: ");
    scanf("%d", &userId);

    User* user = findUserById(userId);
    if (user == NULL || user->memberLevel == USER_ADMIN) {
        printf("用户不存在或无法修改！\n");
        waitForKeyPress();
        return;
    }

    printf("当前用户名: %s\n", user->username);
    printf("当前会员等级: %d\n", user->memberLevel);
    printf("当前消费能力: %.2f\n\n", user->consumptionLevel);

    printf("新密码 (不修改请输入'0'): ");
    char newPassword[50];
    scanf("%s", newPassword);

    printf("新会员等级 (0-新用户, 1-白银会员, 2-黄金会员, 不修改请输入'-1'): ");
    int newMemberLevel;
    scanf("%d", &newMemberLevel);

    // 更新用户信息
    if (strcmp(newPassword, "0") != 0) {
        strcpy(user->password, newPassword);
    }

    if (newMemberLevel >= 0 && newMemberLevel <= 2) {
        user->memberLevel = newMemberLevel;
    }

    printf("用户信息更新成功！\n");
    saveUsersToFile("users.txt");
    waitForKeyPress();
}

// 处理删除用户
void handleDeleteUser() {
    clearScreen();
    printf("=================================\n");
    printf("           删除用户             \n");
    printf("=================================\n");

    int userId;
    printf("请输入要删除的用户ID: ");
    scanf("%d", &userId);

    User* user = findUserById(userId);
    if (user == NULL || user->memberLevel == USER_ADMIN) {
        printf("用户不存在或无法删除！\n");
        waitForKeyPress();
        return;
    }

    printf("确认要删除用户 '%s' 吗？(1-确认，0-取消): ", user->username);
    int confirm;
    scanf("%d", &confirm);

    if (confirm == 1) {
        if (deleteUser(userId)) {
            printf("用户删除成功！\n");
            saveUsersToFile("users.txt");
        }
        else {
            printf("用户删除失败！\n");
        }
    }
    else {
        printf("已取消删除操作。\n");
    }

    waitForKeyPress();
}

// 包裹管理
void handlePackageManagement() {
    int running = 1;
    while (running) {
        clearScreen();
        printf("=================================\n");
        printf("           包裹管理             \n");
        printf("=================================\n");
        printf("1. 查看所有包裹\n");
        printf("2. 添加包裹\n");
        printf("3. 编辑包裹\n");
        printf("4. 标记包裹取出\n");
        printf("5. 标记包裹异常\n");
        printf("6. 搜索包裹\n");
        printf("0. 返回\n");
        printf("请选择操作：");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            displayAllPackages();
            waitForKeyPress();
            break;
        case 2:
            handleAddPackage();
            break;
        case 3:
            handleEditPackage();
            break;
        case 4:
            handleMarkPackagePickedUp();
            break;
        case 5:
            handleMarkPackageAbnormal();
            break;
        case 6:
            handleSearchPackage();
            break;
        case 0:
            showAdminMenu();
            break;
        default:
            printf("无效选择，请重新输入！\n");
            waitForKeyPress();
        }
    }
}

// 显示所有包裹
void displayAllPackages() {
    clearScreen();
    printf("===========================================================\n");
    printf("                        所有包裹                          \n");
    printf("===========================================================\n");
    printf("%-5s %-15s %-8s %-8s %-12s %-10s %-10s\n",
        "ID", "取件码", "用户ID", "大小", "状态", "价值", "创建时间");
    printf("-----------------------------------------------------------\n");

    Package* current = g_packageList;
    int count = 0;

    while (current != NULL) {
        printf("%-5d %-15s %-8d ",
            current->id, current->pickupCode, current->userId);

        // 包裹大小
        switch (current->size) {
        case PACKAGE_SIZE_TINY:
            printf("%-8s ", "极小");
            break;
        case PACKAGE_SIZE_SMALL:
            printf("%-8s ", "小");
            break;
        case PACKAGE_SIZE_MEDIUM:
            printf("%-8s ", "中");
            break;
        case PACKAGE_SIZE_LARGE:
            printf("%-8s ", "大");
            break;
        case PACKAGE_SIZE_HUGE:
            printf("%-8s ", "极大");
            break;
        default:
            printf("%-8s ", "未知");
        }

        // 包裹状态
        switch (current->status) {
        case PACKAGE_STATUS_WAITING:
            printf("%-12s ", "待取");
            break;
        case PACKAGE_STATUS_PICKED:
            printf("%-12s ", "已取");
            break;
        case PACKAGE_STATUS_ABNORMAL:
            printf("%-12s ", "异常");
            break;
        default:
            printf("%-12s ", "未知");
        }

        printf("%-10.2f %-10s\n", current->value, current->createTime);
        count++;

        current = current->next;
    }

    if (count == 0) {
        printf("暂无包裹记录\n");
    }
}

// 处理添加包裹
void handleAddPackage() {
    clearScreen();
    printf("=================================\n");
    printf("           添加包裹             \n");
    printf("=================================\n");

    int userId;
    printf("收件人用户ID: ");
    scanf("%d", &userId);

    // 验证用户是否存在
    User* user = findUserById(userId);
    if (user == NULL || user->memberLevel == USER_ADMIN) {
        printf("用户不存在！\n");
        waitForKeyPress();
        return;
    }

    int size;
    printf("包裹大小 (0-极小, 1-小, 2-中, 3-大, 4-极大): ");
    scanf("%d", &size);
    if (size < 0 || size > 4) {
        printf("无效的包裹大小！\n");
        waitForKeyPress();
        return;
    }

    int weight;
    printf("包裹重量级别 (0-4): ");
    scanf("%d", &weight);
    if (weight < 0 || weight > 4) {
        printf("无效的包裹重量！\n");
        waitForKeyPress();
        return;
    }

    int note;
    printf("包裹备注 (0-无, 1-易碎, 2-冷鲜): ");
    scanf("%d", &note);
    if (note < 0 || note > 2) {
        printf("无效的包裹备注！\n");
        waitForKeyPress();
        return;
    }

    int transportMethod;
    printf("运输方式 (0-正常货车, 1-加快公路, 2-特快空运, 3-特快公路): ");
    scanf("%d", &transportMethod);
    if (transportMethod < 0 || transportMethod > 3) {
        printf("无效的运输方式！\n");
        waitForKeyPress();
        return;
    }

    double value;
    printf("内容物价值: ");
    scanf("%lf", &value);
    if (value < 0) {
        printf("无效的价值！\n");
        waitForKeyPress();
        return;
    }

    // 选择合适的货架
    int shelfId = findSuitableShelf(size, note);
    if (shelfId == -1) {
        printf("没有合适的货架可用！\n");
        waitForKeyPress();
        return;
    }

    // 创建新包裹
    Package* newPackage = addPackage(userId, size, weight, note, transportMethod, value, shelfId);
    if (newPackage == NULL) {
        printf("添加包裹失败！\n");
    }
    else {
        printf("添加包裹成功！取件码: %s\n", newPackage->pickupCode);
        savePackagesToFile("packages.txt");
        saveShelvesToFile("shelves.txt");

        // 添加计件费收入记录
        double fee = calculatePackageFee(size, weight, transportMethod);
        addTransaction(TRANSACTION_INCOME, INCOME_PIECE_FEE, fee, "包裹计件费");
        saveTransactionsToFile("transactions.txt");
    }

    waitForKeyPress();
}

// 货架管理
void handleShelfManagement() {
    int running = 1;
    while (running) {
        clearScreen();
        printf("=================================\n");
        printf("         库存与货架管理         \n");
        printf("=================================\n");
        printf("1. 查看所有货架\n");
        printf("2. 添加货架\n");
        printf("3. 货架库存预警\n");
        printf("4. 库存盘点\n");
        printf("0. 返回\n");
        printf("请选择操作：");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            displayAllShelves();
            waitForKeyPress();
            break;
        case 2:
            handleAddShelf();
            break;
        case 3:
            //handleShelfWarning();
            break;
        case 4:
            //handleInventoryCheck();
            break;
        case 0:
            showAdminMenu();
            break;
        default:
            printf("无效选择，请重新输入！\n");
            waitForKeyPress();
        }
    }
}

// 显示所有货架
void displayAllShelves() {
    clearScreen();
    printf("=================================\n");
    printf("           所有货架             \n");
    printf("=================================\n");
    printf("%-5s %-20s %-10s %-10s %-10s\n", "ID", "类型", "总容量", "已用", "利用率");
    printf("---------------------------------\n");

    Shelf* current = g_shelfList;
    int count = 0;

    while (current != NULL) {
        printf("%-5d ", current->id);

        // 货架类型
        switch (current->type) {
        case 0:
            printf("%-20s ", "极小包裹");
            break;
        case 1:
            printf("%-20s ", "小包裹");
            break;
        case 2:
            printf("%-20s ", "中包裹");
            break;
        case 3:
            printf("%-20s ", "大包裹");
            break;
        case 4:
            printf("%-20s ", "极大包裹");
            break;
        case 5:
            printf("%-20s ", "易碎品");
            break;
        case 6:
            printf("%-20s ", "冷鲜");
            break;
        default:
            printf("%-20s ", "未知");
        }

        printf("%-10d %-10d ", current->capacity, current->currentCount);

        // 计算利用率
        float utilization = (float)current->currentCount / current->capacity * 100;
        printf("%-10.1f%%\n", utilization);

        count++;
        current = current->next;
    }

    if (count == 0) {
        printf("暂无货架记录\n");
    }
}

// 统计分析

// 主函数
int main() {
    // 初始化系统
    initSystem();

    int running = 1;
    while (running) {
        // 显示主菜单
        showMainMenu();

        int choice;
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            handleLogin();
            break;
        case 2:
            handleRegister();
            break;
        case 0:
            running = 0;
            printf("感谢使用快递驿站管理系统！\n");
            break;
        default:
            printf("无效选择，请重新输入！\n");
            waitForKeyPress();
        }
    }

    // 保存数据
    saveAllData();

    return 0;
}
void handleEditPackage() {
    clearScreen();
    printf("=================================\n");
    printf("           编辑包裹             \n");
    printf("=================================\n");

    int packageId;
    printf("请输入包裹ID: ");
    scanf("%d", &packageId);

    Package* package = findPackageById(packageId);
    if (package == NULL) {
        printf("包裹不存在！\n");
        waitForKeyPress();
        return;
    }

    printf("当前包裹信息：\n");
    printf("用户ID: %d\n", package->userId);
    printf("大小: %d\n", package->size);
    printf("重量: %d\n", package->weight);
    printf("备注: %d\n", package->note);
    printf("运输方式: %d\n", package->transportMethod);
    printf("价值: %.2f\n", package->value);
    printf("状态: %d\n\n", package->status);

    printf("功能开发中...\n");
    waitForKeyPress();
}

void handleMarkPackagePickedUp() {
    clearScreen();
    printf("=================================\n");
    printf("         标记包裹已取出         \n");
    printf("=================================\n");

    int packageId;
    printf("请输入包裹ID: ");
    scanf("%d", &packageId);

    if (markPackageAsPickedUp(packageId)) {
        printf("包裹已成功标记为已取出！\n");
        savePackagesToFile("packages.txt");
        saveShelvesToFile("shelves.txt");
    }
    else {
        printf("操作失败，包裹可能不存在或已经取出！\n");
    }

    waitForKeyPress();
}

void handleMarkPackageAbnormal() {
    clearScreen();
    printf("=================================\n");
    printf("         标记包裹异常           \n");
    printf("=================================\n");

    int packageId;
    printf("请输入包裹ID: ");
    scanf("%d", &packageId);

    char reason[100];
    printf("异常原因: ");
    scanf(" %[^\n]", reason);

    if (markPackageAsAbnormal(packageId, reason)) {
        printf("包裹已成功标记为异常！\n");
        savePackagesToFile("packages.txt");
    }
    else {
        printf("操作失败，包裹可能不存在或已经不在待取状态！\n");
    }

    waitForKeyPress();
}

void handleSearchPackage() {
    clearScreen();
    printf("=================================\n");
    printf("           搜索包裹             \n");
    printf("=================================\n");

    printf("1. 按ID搜索\n");
    printf("2. 按取件码搜索\n");
    printf("请选择搜索方式: ");

    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        int packageId;
        printf("请输入包裹ID: ");
        scanf("%d", &packageId);

        Package* package = findPackageById(packageId);
        if (package == NULL) {
            printf("未找到包裹！\n");
        }
        else {
            printf("\n找到包裹：\n");
            printf("ID: %d\n", package->id);
            printf("取件码: %s\n", package->pickupCode);
            printf("用户ID: %d\n", package->userId);
            printf("状态: %d\n", package->status);
            printf("创建时间: %s\n", package->createTime);
        }
    }
    else if (choice == 2) {
        char pickupCode[20];
        printf("请输入取件码: ");
        scanf("%s", pickupCode);

        Package* package = findPackageByCode(pickupCode);
        if (package == NULL) {
            printf("未找到包裹！\n");
        }
        else {
            printf("\n找到包裹：\n");
            printf("ID: %d\n", package->id);
            printf("取件码: %s\n", package->pickupCode);
            printf("用户ID: %d\n", package->userId);
            printf("状态: %d\n", package->status);
            printf("创建时间: %s\n", package->createTime);
        }
    }
    else {
        printf("无效的选择！\n");
    }

    waitForKeyPress();
}

void handleAddShelf() {
    clearScreen();
    printf("=================================\n");
    printf("           添加货架             \n");
    printf("=================================\n");

    int type;
    printf("货架类型 (0-极小包裹, 1-小包裹, 2-中包裹, 3-大包裹, 4-极大包裹, 5-易碎品, 6-冷鲜): ");
    scanf("%d", &type);

    if (type < 0 || type > 6) {
        printf("无效的货架类型！\n");
        waitForKeyPress();
        return;
    }

    int capacity;
    printf("货架容量: ");
    scanf("%d", &capacity);

    if (capacity <= 0) {
        printf("货架容量必须大于0！\n");
        waitForKeyPress();
        return;
    }

    Shelf* newShelf = addShelf(type, capacity);
    if (newShelf == NULL) {
        printf("添加货架失败！\n");
    }
    else {
        printf("添加货架成功！货架ID: %d\n", newShelf->id);
        saveShelvesToFile("shelves.txt");
    }

    waitForKeyPress();
}