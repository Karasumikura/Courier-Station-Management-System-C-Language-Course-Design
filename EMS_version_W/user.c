#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "main.h"
#include "user.h"
#include "util.h"

//用户系统
User* g_userList = NULL;


void initUserList() {
    g_userList = NULL;
}


User* addUser(const char* username, const char* phonenumber, const char* password, int memberLevel) {

    
    User* newUser = (User*)malloc(sizeof(User));
    if (newUser == NULL) {
        return NULL; 
    }

    
    newUser->id = generateUniqueId();
    strncpy(newUser->username, username, sizeof(newUser->username) - 1);
    newUser->username[sizeof(newUser->username) - 1] = '\0';
    strncpy(newUser->phonenumber, phonenumber, sizeof(newUser->phonenumber) - 1);
    newUser->phonenumber[sizeof(newUser->phonenumber) - 1] = '\0';
    strncpy(newUser->password, password, sizeof(newUser->password) - 1);
    newUser->password[sizeof(newUser->password) - 1] = '\0';
    newUser->memberLevel = memberLevel;
    newUser->consumptionLevel = 0.0;
    newUser->Promotionstatus_user = 0;
    newUser->next = NULL;

    
    if (g_userList == NULL) {
        g_userList = newUser;
    }
    else {
        newUser->next = g_userList;
        g_userList = newUser;
    }

    return newUser;
}

void AddUserCheck() {
    char username[50];
    char password[50];
    char confirmPassword[50];
    char phonenumber[13];
    int memberLevel;

   
    while (1) {
        printf("用户名: ");
		getchar();
        if (fgets(username, sizeof(username), stdin) == NULL || strchr(username, '\n') == NULL) {
            
            int buffer;
            while ((buffer = getchar()) != '\n' && buffer != EOF);
            printf("输入非法或过长！！\n");
            waitForKeyPress();
            continue;
        }
        username[strcspn(username, "\n")] = 0; 
        if (strlen(username) == 0) {
            printf("用户名不能为空！！\n");
            waitForKeyPress();
            continue;
        }
        
        if (findUserByUsername(username) != NULL) {
            printf("该用户名已存在！\n");
            waitForKeyPress();
            continue;
        }
        printf("用户名可行性检查通过！！\n");
        break;
    }

   
    while (1) {
        printf("手机号: ");
        if (fgets(phonenumber, sizeof(phonenumber), stdin) == NULL || strchr(phonenumber, '\n') == NULL) {
            
            int buffer;
            while ((buffer = getchar()) != '\n' && buffer != EOF);
            printf("输入非法或过长！！中国大陆手机号为11位数字！\n");
            waitForKeyPress();
            continue;
        }
        phonenumber[strcspn(phonenumber, "\n")] = 0; 

        char* temp = phonenumber;
        int count = 0;
        while (*temp) {
            if (!isdigit(*temp)) {
                printf("手机号只能包含数字！！\n");
                waitForKeyPress();
                break;
            }
            temp++;
            count++;
        }
        if (count != 11) {
            printf("中国大陆手机号为11位数字！！\n");
            waitForKeyPress();
            continue;
        }
        if (findUserByPhone(phonenumber) != NULL) {
            printf("该手机号已注册！注册程序将退出\n");
            waitForKeyPress();
            return;
        }
        printf("手机号可行性检查通过！！\n");
        break; 
    }

    
    printf("密码: ");
    fgets(password, sizeof(password), stdin);
	password[strcspn(password, "\n")] = 0;
    if (strlen(password) == 0) {
        printf("密码不能为空！！\n");
		waitForKeyPress();
        return;
    }

    printf("确认密码: ");
    if (fgets(confirmPassword, sizeof(confirmPassword), stdin) == NULL) {
        printf("输入非法！\n");
		waitForKeyPress();
        return;
    }
    confirmPassword[strcspn(confirmPassword, "\n")] = 0;

    if (strcmp(password, confirmPassword) != 0) {
        printf("两次输入的密码不一致！\n");
        waitForKeyPress();
        return;
    }

    
    User* newUser = addUser(username, phonenumber, password, USER_NEW);
    if (newUser == NULL) {
        printf("注册失败！\n");
    }
    else {
        printf("注册成功！您可以使用新账户登录了。\n");
        saveUsersToFile("users.txt");
    }
    waitForKeyPress();
}

//查找用户的两种方式
User* findUserByUsername(const char* username) {
    User* current = g_userList;

    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}


User* findUserByPhone(const char* phonenumber) {
    User* current = g_userList;

    while (current != NULL) {
        if (strcmp(current->phonenumber, phonenumber) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}


User* findUserById(int userId) {
    User* current = g_userList;

    while (current != NULL) {
        if (current->id == userId) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}


int deleteUser(int userId) {
    if (g_userList == NULL) {
        return 0; 
    }

    
    if (g_userList->id == userId) {
        User* temp = g_userList;
        g_userList = g_userList->next;
        free(temp);
        return 1;
    }

    
    User* current = g_userList;
    while (current->next != NULL && current->next->id != userId) {
        current = current->next;
    }

    
    if (current->next != NULL) {
        User* temp = current->next;
        current->next = temp->next;
        free(temp);
        return 1;
    }

    return 0; 
}

//通过包裹价值来估计用户的消费能力
void updateUserConsumptionLevel(int userId, double packageValue) {
    User* user = findUserById(userId);
    if (user != NULL) {
        
        user->consumptionLevel = (user->consumptionLevel * 0.7) + (packageValue * 0.3);
    }
}


void upgradeUserMemberLevel(int userId) {
    User* user = findUserById(userId);
    if (user != NULL) {
        
        if (user->memberLevel == USER_NEW && user->consumptionLevel >= 500) {
            user->memberLevel = USER_SILVER;
        }
        else if (user->memberLevel == USER_SILVER && user->consumptionLevel >= 1000) {
            user->memberLevel = USER_GOLD;
        }
    }
}

// 将用户数据导出到文件中
void saveUsersToFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法打开用户数据文件 %s\n", filename);
        return;
    }

    User* current = g_userList;
    while (current != NULL) {
        fprintf(file, "%d,%s,%s,%d,%.2f,%s,%d\n",
            current->id,
            current->username,
            current->password,
            current->memberLevel,
            current->consumptionLevel,
            current->phonenumber,
            current->Promotionstatus_user);
        current = current->next;
    }

    fclose(file);
}

// 读取文件中的用户数据
void loadUsersFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("用户数据文件 %s 不存在，将创建新文件\n", filename);
		waitForKeyPress();
        return;
    }

    
    while (g_userList != NULL) {
        User* temp = g_userList;
        g_userList = g_userList->next;
        free(temp);
    }

    
    char line[500];
    while (fgets(line, sizeof(line), file)) {
        int id;
        char username[50];
        char password[50];
        int memberLevel;

        double consumptionLevel;
		char phonenumber[15];
        int Promotionstatus_user;

        if (sscanf(line, "%d,%[^,],%[^,],%d,%lf,%11s,%d",
            &id, username, password, &memberLevel, &consumptionLevel, phonenumber, &Promotionstatus_user) != 7) {
			printf("读取用户数据时格式错误：%s\n", line);
            waitForKeyPress();
        }
        else{
            User* newUser = (User*)malloc(sizeof(User));
            if (newUser != NULL) {
                newUser->id = id;
                strncpy(newUser->username, username, sizeof(newUser->username) - 1);
                newUser->username[sizeof(newUser->username) - 1] = '\0';
                strncpy(newUser->password, password, sizeof(newUser->password) - 1);
                newUser->password[sizeof(newUser->password) - 1] = '\0';
                strncpy(newUser->phonenumber, phonenumber, sizeof(newUser->phonenumber) - 1);
                newUser->phonenumber[sizeof(newUser->phonenumber) - 1] = '\0';
                newUser->memberLevel = memberLevel;
                newUser->consumptionLevel = consumptionLevel;
				newUser->Promotionstatus_user = Promotionstatus_user;

                
                newUser->next = g_userList;
                g_userList = newUser;
            }
        }
    }

    fclose(file);
}