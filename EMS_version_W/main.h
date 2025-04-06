#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 定义各种数据
#define USER_NEW 0       
#define USER_SILVER 1    
#define USER_GOLD 2      
#define USER_ADMIN 3     

#define USER_TYPE_NORMAL 0  
#define USER_TYPE_ADMIN 1   


#define PACKAGE_SIZE_TINY 0
#define PACKAGE_SIZE_SMALL 1
#define PACKAGE_SIZE_MEDIUM 2
#define PACKAGE_SIZE_LARGE 3
#define PACKAGE_SIZE_HUGE 4


#define PACKAGE_NOTE_NONE 0
#define PACKAGE_NOTE_FRAGILE 1
#define PACKAGE_NOTE_FRESH 2


#define TRANSPORT_NORMAL 0        
#define TRANSPORT_FAST_ROAD 1     
#define TRANSPORT_EXPRESS_AIR 2   
#define TRANSPORT_EXPRESS_ROAD 3  


#define PACKAGE_STATUS_WAITING 0  
#define PACKAGE_STATUS_PICKED 1   
#define PACKAGE_STATUS_ABNORMAL 2 


#define TRANSACTION_INCOME 0      
#define TRANSACTION_EXPENSE 1     


#define INCOME_PIECE_FEE 0        
#define INCOME_STORAGE_FEE 1      
#define INCOME_DOORSTEP_FEE 2


#define EXPENSE_COMPENSATION 0    
#define EXPENSE_MAINTENANCE 1     


typedef struct User {
	int id;                  
	char username[50];       
	char password[50];       
	char phonenumber[15];   
	int memberLevel;         
	double consumptionLevel; 
	int Promotionstatus_user;
	struct User* next;       
} User;


typedef struct Package {
	int id;                  
	int userId;              
	int size;                
	int weight;              
	int note;                
	int transportMethod;     
	double value;           
	char pickupCode[20];     
	int shelfId;             
	int status;              
	char createTime[20];     
	struct Package* next;    
} Package;


typedef struct Shelf {
	int id;                  
	int type;                
	int capacity;            
	int currentCount;        
	struct Shelf* next;      
} Shelf;


typedef struct Transaction {
	int id;                  
	int type;                
	int subType;             
	double amount;           
	char description[100];   
	char createTime[20];     
	struct Transaction* next;
} Transaction;


extern User* g_userList;
extern Package* g_packageList;
extern Shelf* g_shelfList;
extern Transaction* g_transactionList;


extern int g_currentUserId;
extern int g_currentUserType;


extern int Promotionstatus;

void clearScreen();
void waitForKeyPress();
void initSystem();
void showMainMenu();
void showAdminMenu();
void showUserMenu();

void clearInputBuffer();

#endif /* MAIN_H */