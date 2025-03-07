#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 常量定义
#define USER_NEW 0       // 新用户
#define USER_SILVER 1    // 白银会员
#define USER_GOLD 2      // 黄金会员
#define USER_ADMIN 3     // 管理员

#define USER_TYPE_NORMAL 0  // 普通用户类型
#define USER_TYPE_ADMIN 1   // 管理员类型

// 包裹大小定义
#define PACKAGE_SIZE_TINY 0
#define PACKAGE_SIZE_SMALL 1
#define PACKAGE_SIZE_MEDIUM 2
#define PACKAGE_SIZE_LARGE 3
#define PACKAGE_SIZE_HUGE 4

// 包裹属性定义
#define PACKAGE_NOTE_NONE 0
#define PACKAGE_NOTE_FRAGILE 1
#define PACKAGE_NOTE_FRESH 2

// 包裹运输方式定义
#define TRANSPORT_NORMAL 0        // 正常（货车）
#define TRANSPORT_FAST_ROAD 1     // 加快（公路）
#define TRANSPORT_EXPRESS_AIR 2   // 特快（空运）
#define TRANSPORT_EXPRESS_ROAD 3  // 特快（公路）

// 包裹状态定义
#define PACKAGE_STATUS_WAITING 0  // 待取
#define PACKAGE_STATUS_PICKED 1   // 已取
#define PACKAGE_STATUS_ABNORMAL 2 // 异常

// 交易类型定义
#define TRANSACTION_INCOME 0      // 收入
#define TRANSACTION_EXPENSE 1     // 支出

// 收入子类型
#define INCOME_PIECE_FEE 0        // 计件费
#define INCOME_STORAGE_FEE 1      // 保存费

// 支出子类型
#define EXPENSE_COMPENSATION 0    // 赔偿金
#define EXPENSE_MAINTENANCE 1     // 维护费

// 用户结构体
typedef struct User {
	int id;                  // 用户ID
	char username[50];       // 用户名
	char password[50];       // 密码
	char phonenumber[15];    // 电话号码
	int memberLevel;         // 会员等级
	double consumptionLevel; // 消费能力等级
	struct User* next;       // 链表下一节点
} User;

// 包裹结构体
typedef struct Package {
	int id;                  // 包裹ID
	int userId;              // 所属用户ID
	int size;                // 大小
	int weight;              // 重量等级：0-4
	int note;                // 备注属性
	int transportMethod;     // 运输方式
	double value;            // 内容物价值
	char pickupCode[20];     // 取件码
	int shelfId;             // 存放货架ID
	int status;              // 状态
	char createTime[20];     // 创建时间
	struct Package* next;    // 链表下一节点
} Package;

// 货架结构体
typedef struct Shelf {
	int id;                  // 货架ID
	int type;                // 类型（基于大小和特性）
	int capacity;            // 总容量
	int currentCount;        // 当前存放数量
	struct Shelf* next;      // 链表下一节点
} Shelf;

// 业务记录结构体
typedef struct Transaction {
	int id;                  // 记录ID
	int type;                // 类型：0-收入，1-支出
	int subType;             // 子类型
	double amount;           // 金额
	char description[100];   // 描述
	char createTime[20];     // 创建时间
	struct Transaction* next;// 链表下一节点
} Transaction;

// 全局链表头声明
extern User* g_userList;
extern Package* g_packageList;
extern Shelf* g_shelfList;
extern Transaction* g_transactionList;

// 当前登录信息
extern int g_currentUserId;
extern int g_currentUserType;

// 系统操作函数
void clearScreen();
void waitForKeyPress();
void initSystem();
void showMainMenu();
void showAdminMenu();
void showUserMenu();

void clearInputBuffer();

#endif /* MAIN_H */