#ifndef AUTH_H
#define AUTH_H

#include "main.h"

// µÇÂ¼ÏµÍ³
int login(const char* username, const char* password);

int adminLogin(const char* username, const char* password);


int authenticateUser(const char* username, const char* password);


void changePassword();




int isLoggedIn();

int isAdminLoggedIn();


void logout();

#endif /* AUTH_H */
