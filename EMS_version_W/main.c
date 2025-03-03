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

// ��ʱ����
int markPackageAsPickedUp(int packageId);
int markPackageAsAbnormal(int packageId, const char* reason);
double calculatePackageFee(int size, int weight, int transportMethod);

// ϵͳ��������
void clearScreen();
void waitForKeyPress();
void initSystem();
void showMainMenu();
void showAdminMenu();
void showUserMenu();

// ��¼���û�ϵͳ����
void handleRegister();
void handleLogin();

// �û�������
void handleUserManagement();
void displayAllUsers();
void handleAddUser();
void handleEditUser();
void handleDeleteUser();

// ����������
void handlePackageManagement();
void displayAllPackages();
void handleAddPackage();
void handleEditPackage();
void handleMarkPackagePickedUp();
void handleMarkPackageAbnormal();
void handleSearchPackage();

// ���ܹ�����
void displayAllShelves();
void handleAddShelf();
void handleShelfManagement();

// ��������
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// �ȴ��û������������
void waitForKeyPress() {
    printf("\n�����������...");
    getchar();
    getchar(); //��������
}

// ��ʼ��ϵͳ
void initSystem() {
    // ��ʼ����������
    initUserList();
    initPackageList();
    initShelfList();
    initTransactionList();

    // ��������
    loadAllData();

    // �������Ϊ�գ�����Ĭ�ϻ���
    if (g_shelfList == NULL) {
        // ����5�ֳߴ�ĳ������
        for (int i = 0; i < 5; i++) {
            addShelf(i, 20); // ÿ�ֳߴ�20������
        }

        // ����������ܣ�����Ʒ
        addShelf(5, 10); // ����Ʒ���ܣ�����10

        // ����������ܣ�����
        addShelf(6, 10); // ���ʻ��ܣ�����10
    }

    // ���û�й���Ա�˻�������Ĭ�Ϲ���Ա
    if (findUserByUsername("admin") == NULL) {
        addUser("admin", "123456", USER_ADMIN);
    }
}




// ��ʾ���˵�
void showMainMenu() {
    clearScreen();
    printf("=================================\n");
    printf("    ��ӭʹ�ÿ����վ����ϵͳ    \n");
    printf("=================================\n");
    printf("1. ��¼\n");
    printf("2. ע�����û�\n");
    printf("0. �˳�ϵͳ\n");
    printf("��ѡ�������");
}

// ��ʾ����Ա�˵�
void showAdminMenu() {
    int choice;
    clearScreen();
    printf("=================================\n");
    printf("    �����վ����ϵͳ - ����Ա    \n");
    printf("=================================\n");
    printf("1. �û�����\n");
    printf("2. ��������\n");
    printf("3. �������ܹ���\n");
    printf("4. ���ݷ����뱨��\n");
    printf("5. ���׼�¼\n");
    printf("0. �ǳ�\n");
    printf("��ѡ�������");
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
		printf("��Чѡ�����������룡\n");
		waitForKeyPress();
	}
}

// ��ʾ�û��˵�
void showUserMenu() {
    int choice;
    clearScreen();
    User* currentUser = findUserById(g_currentUserId);

    printf("=================================\n");
    printf("    �����վ����ϵͳ - �û�    \n");
    printf("=================================\n");
    printf("��ǰ�û���%s (", currentUser->username);

    // ��ʾ��Ա�ȼ�
    switch (currentUser->memberLevel) {
    case USER_NEW:
        printf("���û�");
        break;
    case USER_SILVER:
        printf("������Ա");
        break;
    case USER_GOLD:
        printf("�ƽ��Ա");
        break;
    }
    printf(")\n\n");

    printf("1. �鿴�ҵİ���\n");
    printf("2. ȡ��\n");
    printf("3. �ļ�\n");
    printf("4. �鿴�ҵĻ�Ա��Ϣ\n");
    printf("5. �˻�����\n");
    printf("0. �ǳ�\n");
    printf("��ѡ�������");
	scanf("%d", &choice);
    switch (choice) {
    case 1:
        //displayMyPackages();
        break;
    case 0:
        logout();
        break;
    default:
        printf("��Чѡ�����������룡\n");
        waitForKeyPress();
    }
}

// �����¼
    
    void handleLogin() {
        clearScreen();
        printf("=================================\n");
        printf("             ��¼               \n");
        printf("=================================\n");

        char username[50];
        char password[50];

        printf("�˻�: ");
        scanf("%s", username);
        printf("����: ");
        scanf("%s", password);

        int result = login(username, password);

        if (result == 1) {
            printf("����Ա��¼�ɹ���\n");
            waitForKeyPress();
            showAdminMenu();
        }
        else if (result == 2) {
            printf("�û���¼�ɹ���\n");
            waitForKeyPress();
            showUserMenu();
        }
        else if (result == -1) {
            printf("�˻������ڣ�\n");
            waitForKeyPress();
        }
        else if (result == 0) {
            printf("�������\n");
            waitForKeyPress();
        }
        else {
            printf("��¼ʧ�ܣ�\n");
            waitForKeyPress();
        }
    }



// ����ע��
void handleRegister() {
    clearScreen();
    printf("=================================\n");
    printf("             ע��               \n");
    printf("=================================\n");

    char username[50];
    char password[50];
    char confirmPassword[50];

    printf("�û���: ");
    scanf("%s", username);

    // ����û����Ƿ����
    if (findUserByUsername(username) != NULL) {
        printf("���û����Ѵ��ڣ�\n");
        waitForKeyPress();
        return;
    }

    printf("����: ");
    scanf("%s", password);
    printf("ȷ������: ");
    scanf("%s", confirmPassword);

    if (strcmp(password, confirmPassword) != 0) {
        printf("������������벻һ�£�\n");
        waitForKeyPress();
        return;
    }

    // �������û�
    User* newUser = addUser(username, password, USER_NEW);
    if (newUser == NULL) {
        printf("ע��ʧ�ܣ�\n");
    }
    else {
        printf("ע��ɹ���������ʹ�����˻���¼�ˡ�\n");
        saveUsersToFile("users.txt");
    }

    waitForKeyPress();
}



// �û�����
void handleUserManagement() {
    int running = 1;
    while (running) {
        clearScreen();
        printf("=================================\n");
        printf("           �û�����             \n");
        printf("=================================\n");
        printf("1. �鿴�����û�\n");
        printf("2. ����û�\n");
        printf("3. �༭�û�\n");
        printf("4. ɾ���û�\n");
        printf("0. ����\n");
        printf("��ѡ�������");

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
            printf("��Чѡ�����������룡\n");
            waitForKeyPress();
        }
    }
}

// ��ʾ�����û�
void displayAllUsers() {
    clearScreen();
    printf("=================================\n");
    printf("           �����û�             \n");
    printf("=================================\n");
    printf("%-5s %-20s %-15s %-15s\n", "ID", "�û���", "��Ա�ȼ�", "��������");
    printf("---------------------------------\n");

    User* current = g_userList;
    int count = 0;

    while (current != NULL) {
        // ����ʾ����Ա�˻�
        if (current->memberLevel != USER_ADMIN) {
            printf("%-5d %-20s ", current->id, current->username);

            // ��Ա�ȼ�
            switch (current->memberLevel) {
            case USER_NEW:
                printf("%-15s ", "���û�");
                break;
            case USER_SILVER:
                printf("%-15s ", "������Ա");
                break;
            case USER_GOLD:
                printf("%-15s ", "�ƽ��Ա");
                break;
            default:
                printf("%-15s ", "δ֪");
            }

            printf("%-15.2f\n", current->consumptionLevel);
            count++;
        }

        current = current->next;
    }

    if (count == 0) {
        printf("������ͨ�û���¼\n");
    }
}

// ��������û�
void handleAddUser() {
    clearScreen();
    printf("=================================\n");
    printf("           ����û�             \n");
    printf("=================================\n");

    char username[50];
    char password[50];
    int memberLevel;

    printf("�û���: ");
    scanf("%s", username);

    // ����û����Ƿ����
    if (findUserByUsername(username) != NULL) {
        printf("���û����Ѵ��ڣ�\n");
        waitForKeyPress();
        return;
    }

    printf("����: ");
    scanf("%s", password);

    printf("��Ա�ȼ� (0-���û�, 1-������Ա, 2-�ƽ��Ա): ");
    scanf("%d", &memberLevel);

    if (memberLevel < 0 || memberLevel > 2) {
        printf("��Ч�Ļ�Ա�ȼ���\n");
        waitForKeyPress();
        return;
    }

    // �������û�
    User* newUser = addUser(username, password, memberLevel);
    if (newUser == NULL) {
        printf("����û�ʧ�ܣ�\n");
    }
    else {
        printf("����û��ɹ���\n");
        saveUsersToFile("users.txt");
    }

    waitForKeyPress();
}

// ����༭�û�
void handleEditUser() {
    clearScreen();
    printf("=================================\n");
    printf("           �༭�û�             \n");
    printf("=================================\n");

    int userId;
    printf("������Ҫ�༭���û�ID: ");
    scanf("%d", &userId);

    User* user = findUserById(userId);
    if (user == NULL || user->memberLevel == USER_ADMIN) {
        printf("�û������ڻ��޷��޸ģ�\n");
        waitForKeyPress();
        return;
    }

    printf("��ǰ�û���: %s\n", user->username);
    printf("��ǰ��Ա�ȼ�: %d\n", user->memberLevel);
    printf("��ǰ��������: %.2f\n\n", user->consumptionLevel);

    printf("������ (���޸�������'0'): ");
    char newPassword[50];
    scanf("%s", newPassword);

    printf("�»�Ա�ȼ� (0-���û�, 1-������Ա, 2-�ƽ��Ա, ���޸�������'-1'): ");
    int newMemberLevel;
    scanf("%d", &newMemberLevel);

    // �����û���Ϣ
    if (strcmp(newPassword, "0") != 0) {
        strcpy(user->password, newPassword);
    }

    if (newMemberLevel >= 0 && newMemberLevel <= 2) {
        user->memberLevel = newMemberLevel;
    }

    printf("�û���Ϣ���³ɹ���\n");
    saveUsersToFile("users.txt");
    waitForKeyPress();
}

// ����ɾ���û�
void handleDeleteUser() {
    clearScreen();
    printf("=================================\n");
    printf("           ɾ���û�             \n");
    printf("=================================\n");

    int userId;
    printf("������Ҫɾ�����û�ID: ");
    scanf("%d", &userId);

    User* user = findUserById(userId);
    if (user == NULL || user->memberLevel == USER_ADMIN) {
        printf("�û������ڻ��޷�ɾ����\n");
        waitForKeyPress();
        return;
    }

    printf("ȷ��Ҫɾ���û� '%s' ��(1-ȷ�ϣ�0-ȡ��): ", user->username);
    int confirm;
    scanf("%d", &confirm);

    if (confirm == 1) {
        if (deleteUser(userId)) {
            printf("�û�ɾ���ɹ���\n");
            saveUsersToFile("users.txt");
        }
        else {
            printf("�û�ɾ��ʧ�ܣ�\n");
        }
    }
    else {
        printf("��ȡ��ɾ��������\n");
    }

    waitForKeyPress();
}

// ��������
void handlePackageManagement() {
    int running = 1;
    while (running) {
        clearScreen();
        printf("=================================\n");
        printf("           ��������             \n");
        printf("=================================\n");
        printf("1. �鿴���а���\n");
        printf("2. ��Ӱ���\n");
        printf("3. �༭����\n");
        printf("4. ��ǰ���ȡ��\n");
        printf("5. ��ǰ����쳣\n");
        printf("6. ��������\n");
        printf("0. ����\n");
        printf("��ѡ�������");

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
            printf("��Чѡ�����������룡\n");
            waitForKeyPress();
        }
    }
}

// ��ʾ���а���
void displayAllPackages() {
    clearScreen();
    printf("===========================================================\n");
    printf("                        ���а���                          \n");
    printf("===========================================================\n");
    printf("%-5s %-15s %-8s %-8s %-12s %-10s %-10s\n",
        "ID", "ȡ����", "�û�ID", "��С", "״̬", "��ֵ", "����ʱ��");
    printf("-----------------------------------------------------------\n");

    Package* current = g_packageList;
    int count = 0;

    while (current != NULL) {
        printf("%-5d %-15s %-8d ",
            current->id, current->pickupCode, current->userId);

        // ������С
        switch (current->size) {
        case PACKAGE_SIZE_TINY:
            printf("%-8s ", "��С");
            break;
        case PACKAGE_SIZE_SMALL:
            printf("%-8s ", "С");
            break;
        case PACKAGE_SIZE_MEDIUM:
            printf("%-8s ", "��");
            break;
        case PACKAGE_SIZE_LARGE:
            printf("%-8s ", "��");
            break;
        case PACKAGE_SIZE_HUGE:
            printf("%-8s ", "����");
            break;
        default:
            printf("%-8s ", "δ֪");
        }

        // ����״̬
        switch (current->status) {
        case PACKAGE_STATUS_WAITING:
            printf("%-12s ", "��ȡ");
            break;
        case PACKAGE_STATUS_PICKED:
            printf("%-12s ", "��ȡ");
            break;
        case PACKAGE_STATUS_ABNORMAL:
            printf("%-12s ", "�쳣");
            break;
        default:
            printf("%-12s ", "δ֪");
        }

        printf("%-10.2f %-10s\n", current->value, current->createTime);
        count++;

        current = current->next;
    }

    if (count == 0) {
        printf("���ް�����¼\n");
    }
}

// ������Ӱ���
void handleAddPackage() {
    clearScreen();
    printf("=================================\n");
    printf("           ��Ӱ���             \n");
    printf("=================================\n");

    int userId;
    printf("�ռ����û�ID: ");
    scanf("%d", &userId);

    // ��֤�û��Ƿ����
    User* user = findUserById(userId);
    if (user == NULL || user->memberLevel == USER_ADMIN) {
        printf("�û������ڣ�\n");
        waitForKeyPress();
        return;
    }

    int size;
    printf("������С (0-��С, 1-С, 2-��, 3-��, 4-����): ");
    scanf("%d", &size);
    if (size < 0 || size > 4) {
        printf("��Ч�İ�����С��\n");
        waitForKeyPress();
        return;
    }

    int weight;
    printf("������������ (0-4): ");
    scanf("%d", &weight);
    if (weight < 0 || weight > 4) {
        printf("��Ч�İ���������\n");
        waitForKeyPress();
        return;
    }

    int note;
    printf("������ע (0-��, 1-����, 2-����): ");
    scanf("%d", &note);
    if (note < 0 || note > 2) {
        printf("��Ч�İ�����ע��\n");
        waitForKeyPress();
        return;
    }

    int transportMethod;
    printf("���䷽ʽ (0-��������, 1-�ӿ칫·, 2-�ؿ����, 3-�ؿ칫·): ");
    scanf("%d", &transportMethod);
    if (transportMethod < 0 || transportMethod > 3) {
        printf("��Ч�����䷽ʽ��\n");
        waitForKeyPress();
        return;
    }

    double value;
    printf("�������ֵ: ");
    scanf("%lf", &value);
    if (value < 0) {
        printf("��Ч�ļ�ֵ��\n");
        waitForKeyPress();
        return;
    }

    // ѡ����ʵĻ���
    int shelfId = findSuitableShelf(size, note);
    if (shelfId == -1) {
        printf("û�к��ʵĻ��ܿ��ã�\n");
        waitForKeyPress();
        return;
    }

    // �����°���
    Package* newPackage = addPackage(userId, size, weight, note, transportMethod, value, shelfId);
    if (newPackage == NULL) {
        printf("��Ӱ���ʧ�ܣ�\n");
    }
    else {
        printf("��Ӱ����ɹ���ȡ����: %s\n", newPackage->pickupCode);
        savePackagesToFile("packages.txt");
        saveShelvesToFile("shelves.txt");

        // ��ӼƼ��������¼
        double fee = calculatePackageFee(size, weight, transportMethod);
        addTransaction(TRANSACTION_INCOME, INCOME_PIECE_FEE, fee, "�����Ƽ���");
        saveTransactionsToFile("transactions.txt");
    }

    waitForKeyPress();
}

// ���ܹ���
void handleShelfManagement() {
    int running = 1;
    while (running) {
        clearScreen();
        printf("=================================\n");
        printf("         �������ܹ���         \n");
        printf("=================================\n");
        printf("1. �鿴���л���\n");
        printf("2. ��ӻ���\n");
        printf("3. ���ܿ��Ԥ��\n");
        printf("4. ����̵�\n");
        printf("0. ����\n");
        printf("��ѡ�������");

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
            printf("��Чѡ�����������룡\n");
            waitForKeyPress();
        }
    }
}

// ��ʾ���л���
void displayAllShelves() {
    clearScreen();
    printf("=================================\n");
    printf("           ���л���             \n");
    printf("=================================\n");
    printf("%-5s %-20s %-10s %-10s %-10s\n", "ID", "����", "������", "����", "������");
    printf("---------------------------------\n");

    Shelf* current = g_shelfList;
    int count = 0;

    while (current != NULL) {
        printf("%-5d ", current->id);

        // ��������
        switch (current->type) {
        case 0:
            printf("%-20s ", "��С����");
            break;
        case 1:
            printf("%-20s ", "С����");
            break;
        case 2:
            printf("%-20s ", "�а���");
            break;
        case 3:
            printf("%-20s ", "�����");
            break;
        case 4:
            printf("%-20s ", "�������");
            break;
        case 5:
            printf("%-20s ", "����Ʒ");
            break;
        case 6:
            printf("%-20s ", "����");
            break;
        default:
            printf("%-20s ", "δ֪");
        }

        printf("%-10d %-10d ", current->capacity, current->currentCount);

        // ����������
        float utilization = (float)current->currentCount / current->capacity * 100;
        printf("%-10.1f%%\n", utilization);

        count++;
        current = current->next;
    }

    if (count == 0) {
        printf("���޻��ܼ�¼\n");
    }
}

// ͳ�Ʒ���

// ������
int main() {
    // ��ʼ��ϵͳ
    initSystem();

    int running = 1;
    while (running) {
        // ��ʾ���˵�
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
            printf("��лʹ�ÿ����վ����ϵͳ��\n");
            break;
        default:
            printf("��Чѡ�����������룡\n");
            waitForKeyPress();
        }
    }

    // ��������
    saveAllData();

    return 0;
}
void handleEditPackage() {
    clearScreen();
    printf("=================================\n");
    printf("           �༭����             \n");
    printf("=================================\n");

    int packageId;
    printf("���������ID: ");
    scanf("%d", &packageId);

    Package* package = findPackageById(packageId);
    if (package == NULL) {
        printf("���������ڣ�\n");
        waitForKeyPress();
        return;
    }

    printf("��ǰ������Ϣ��\n");
    printf("�û�ID: %d\n", package->userId);
    printf("��С: %d\n", package->size);
    printf("����: %d\n", package->weight);
    printf("��ע: %d\n", package->note);
    printf("���䷽ʽ: %d\n", package->transportMethod);
    printf("��ֵ: %.2f\n", package->value);
    printf("״̬: %d\n\n", package->status);

    printf("���ܿ�����...\n");
    waitForKeyPress();
}

void handleMarkPackagePickedUp() {
    clearScreen();
    printf("=================================\n");
    printf("         ��ǰ�����ȡ��         \n");
    printf("=================================\n");

    int packageId;
    printf("���������ID: ");
    scanf("%d", &packageId);

    if (markPackageAsPickedUp(packageId)) {
        printf("�����ѳɹ����Ϊ��ȡ����\n");
        savePackagesToFile("packages.txt");
        saveShelvesToFile("shelves.txt");
    }
    else {
        printf("����ʧ�ܣ��������ܲ����ڻ��Ѿ�ȡ����\n");
    }

    waitForKeyPress();
}

void handleMarkPackageAbnormal() {
    clearScreen();
    printf("=================================\n");
    printf("         ��ǰ����쳣           \n");
    printf("=================================\n");

    int packageId;
    printf("���������ID: ");
    scanf("%d", &packageId);

    char reason[100];
    printf("�쳣ԭ��: ");
    scanf(" %[^\n]", reason);

    if (markPackageAsAbnormal(packageId, reason)) {
        printf("�����ѳɹ����Ϊ�쳣��\n");
        savePackagesToFile("packages.txt");
    }
    else {
        printf("����ʧ�ܣ��������ܲ����ڻ��Ѿ����ڴ�ȡ״̬��\n");
    }

    waitForKeyPress();
}

void handleSearchPackage() {
    clearScreen();
    printf("=================================\n");
    printf("           ��������             \n");
    printf("=================================\n");

    printf("1. ��ID����\n");
    printf("2. ��ȡ��������\n");
    printf("��ѡ��������ʽ: ");

    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        int packageId;
        printf("���������ID: ");
        scanf("%d", &packageId);

        Package* package = findPackageById(packageId);
        if (package == NULL) {
            printf("δ�ҵ�������\n");
        }
        else {
            printf("\n�ҵ�������\n");
            printf("ID: %d\n", package->id);
            printf("ȡ����: %s\n", package->pickupCode);
            printf("�û�ID: %d\n", package->userId);
            printf("״̬: %d\n", package->status);
            printf("����ʱ��: %s\n", package->createTime);
        }
    }
    else if (choice == 2) {
        char pickupCode[20];
        printf("������ȡ����: ");
        scanf("%s", pickupCode);

        Package* package = findPackageByCode(pickupCode);
        if (package == NULL) {
            printf("δ�ҵ�������\n");
        }
        else {
            printf("\n�ҵ�������\n");
            printf("ID: %d\n", package->id);
            printf("ȡ����: %s\n", package->pickupCode);
            printf("�û�ID: %d\n", package->userId);
            printf("״̬: %d\n", package->status);
            printf("����ʱ��: %s\n", package->createTime);
        }
    }
    else {
        printf("��Ч��ѡ��\n");
    }

    waitForKeyPress();
}

void handleAddShelf() {
    clearScreen();
    printf("=================================\n");
    printf("           ��ӻ���             \n");
    printf("=================================\n");

    int type;
    printf("�������� (0-��С����, 1-С����, 2-�а���, 3-�����, 4-�������, 5-����Ʒ, 6-����): ");
    scanf("%d", &type);

    if (type < 0 || type > 6) {
        printf("��Ч�Ļ������ͣ�\n");
        waitForKeyPress();
        return;
    }

    int capacity;
    printf("��������: ");
    scanf("%d", &capacity);

    if (capacity <= 0) {
        printf("���������������0��\n");
        waitForKeyPress();
        return;
    }

    Shelf* newShelf = addShelf(type, capacity);
    if (newShelf == NULL) {
        printf("��ӻ���ʧ�ܣ�\n");
    }
    else {
        printf("��ӻ��ܳɹ�������ID: %d\n", newShelf->id);
        saveShelvesToFile("shelves.txt");
    }

    waitForKeyPress();
}