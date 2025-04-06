#ifndef USER_H
#define USER_H

#include "main.h"

//用户系统
void initUserList();

User* addUser(const char* username, const char*phonenumber , const char* password, int memberLevel);


void AddUserCheck();


User* findUserByUsername(const char* username);


User* findUserById(int userId);

User* findUserByPhone(const char* phonenumber);



int deleteUser(int userId);



void updateUserConsumptionLevel(int userId, double packageValue);

void upgradeUserMemberLevel(int userId);


void saveUsersToFile(const char* filename);


void loadUsersFromFile(const char* filename);


//void freeUserList();

#endif /* USER_H */
