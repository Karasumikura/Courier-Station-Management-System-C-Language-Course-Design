#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>//_isdigit()
#include <conio.h>//_getch()
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


double calculatePackageFee(int size, int weight, int transportMethod);

// ��¼���û�ϵͳ����
void handleRegister();
void handleLogin();

// �û�������
void handleUserManagement();
void displayAllUsers();
void handleAddUser();
void handleEditUser();
void handleDeleteUser();
void displayMyProfile();

// ����������
void handlePackageManagement();
void displayAllPackages();
void handleAddPackage();
void handleEditPackage();
void handleMarkPackagePickedUp();
void handleMarkPackageAbnormal();
void handleSearchPackage();
void handlePickupPackage();

// ���ܹ�����
void displayAllShelves();
void handleAddShelf();
void handleShelfManagement();
void handleAddShelf();
void handleTransactions();

void handleStatistics();

//��������
void displayPromotions();

//���������
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");// ֧�ֿ�ƽ̨����-
#endif
}

void waitForKeyPress() {
    printf("\n�����������...\n");
    _getch();  // ����ֱ�Ӷ�ȡһ���ַ�������Ҫ�û����»س���-
    //���Ҳ��Ὣ������ַ����Ե���Ļ��-
	printf("\n");
}

void initSystem() {
    initUserList();
    initPackageList();
    initShelfList();
    initTransactionList();

    loadAllData();

    if (g_shelfList == NULL) {
        // �������
        for (int i = 0; i < 5; i++) {
            addShelf(i, 20); // ÿ�ֳߴ�20������
        }

        // �����������Ʒ
        addShelf(5, 10); 

        // �����������
        addShelf(6, 10); 
    }

    // ���û�й���Ա������Ĭ�Ϲ���Ա
    if (findUserByUsername("admin") == NULL) {
        addUser("admin", "123456", "123456", USER_ADMIN);
    }
}




// ��ʾ���˵�
void showMainMenu() {
    clearScreen();
    int running = 1;
    printf("=================================\n");
    printf("    ��ӭʹ�ÿ����վ����ϵͳ    \n");
    printf("=================================\n");
    printf("1. ��¼\n");
    printf("2. ע�����û�\n");
    printf("0. �˳�ϵͳ\n");
}

// ��ʾ����Ա�˵�
void showAdminMenu() {
    int choice;
    int running = 1;
    while (running) {
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
            handleStatistics();
            break;
        case 5:
            handleTransactions();
            break;
        case 0:
            logout();
            running = 0;
            break;
        default:
            printf("��Чѡ�����������룡\n");
            waitForKeyPress();
        }
    }
    showMainMenu();
}

void handleStatistics() {
    char report[1024];
		clearScreen();
		printf("=================================\n");
		printf("    ���ݷ����뱨��    \n");
		printf("=================================\n");
		printf("1. �ձ�\n");
		printf("2. �ܱ�\n");
		printf("3. �±�\n");
		printf("0. ����\n");
		printf("��ѡ�������");
		int choice;
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			generateDailyReport(report);
			break;
		case 2:
			
			break;
		case 3:

			break;
		case 0:
			break;
		default:
			printf("��Чѡ�����������룡\n");
			waitForKeyPress();
		}
        printf("%s\n",report);
}

void handleTransactions() {
	int running = 1;
    while (running) {
        clearScreen();
        printf("=================================\n");
        printf("           ���׼�¼             \n");
        printf("=================================\n");
        printf("1. �鿴���н��׼�¼\n");
        printf("2. �������׼�¼\n");
        printf("0. ����\n");
        printf("��ѡ�������");
        int choice;
        scanf("%d", &choice);
        switch (choice) {
        case 1:
          
            waitForKeyPress();
            break;
        case 2:
            
            break;
        
        case 0:
            running = 0;
            break;
        default:
            printf("��Чѡ�����������룡\n");
            waitForKeyPress();
        }
        showAdminMenu();
    }
}
// ��ʾ�û��˵�
void showUserMenu() {
    int choice;
    int running = 1;
    while (running) {
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
        if (currentUser->Promotionstatus_user) {
            timecheck();//ʱЧ�Լ��
        }
        switch (Promotionstatus)
        {
        case 1:
			printf("�����ڻ�У�\n");
            break;
        default:
			break;
        }
        Package** userPackages;
		int count;
        printf("1. �鿴�ҵĴ�ȡ����\n");
        printf("2. ȡ��\n");
        printf("3. �ļ�\n");
        printf("4. �鿴�ҵ���Ϣ\n");
        printf("5. �鿴�\n");
        printf("0. �ǳ�\n");
        printf("��ѡ�������");
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            userPackages = getUserWaitingPackages(currentUser->id,&count);
			printUserPackages(userPackages, count);
            break;
        case 2:
            handlePickupPackage();
            break;
        case 3:
            handleAddPackage();
            break;
        case 4:
            displayMyProfile();
            break;
        case 5:
			displayPromotions();
			break;
        case 0:
            running = 0;
            logout();
            break;
        default:
            printf("��Чѡ�����������룡\n");
            waitForKeyPress();
        }
    }
    showMainMenu();
}

void displayPromotions() {
    User* currentUser = findUserById(g_currentUserId);
    time_t raw_time;
    time(&raw_time);
    struct tm* local_time = localtime(&raw_time);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local_time);
	clearScreen();
	printf("=================================\n");
	printf("           ���Ϣ             \n");
	printf("=================================\n");
	printf("��վ�Żݻʱ�䣺2025.03.21 - 2025.06.21�������£�\n");
	printf("����ݣ����û�5%%�ۿۣ�������Ա10%%�ۿۣ��ƽ��Ա20%%�ۿۣ�����\n");
    printf("��ǰʱ��: %s\n", buffer);
    timecheck();

    if (Promotionstatus) {
        printf("������У�\n");
        switch (currentUser->memberLevel) {
        case USER_NEW:
            printf("�������û�������5%%�ۿۣ�������Ч\n");
            break;
        case USER_SILVER:
            printf("���ǰ�����Ա������10%%�ۿۣ�������Ч\n");
            break;
        case USER_GOLD:
            printf("���ǻƽ��Ա������20%%�ۿۣ�������Ч\n");
            break;
        }
    }
    else {
        printf("��Ѿ���ʱ\n");
    }

	waitForKeyPress();
}
void displayMyProfile() {
    int choice;
	clearScreen();
	User* currentUser = findUserById(g_currentUserId);
	printf("=================================\n");
	printf("           �ҵ���Ϣ             \n");
	printf("=================================\n");
	printf("�û���: %s\n", currentUser->username);
	printf("��Ա�ȼ�: ");
	switch (currentUser->memberLevel) {
	case USER_NEW:
		printf("���û�\n");
		break;
	case USER_SILVER:
		printf("������Ա\n");
		break;
	case USER_GOLD:
		printf("�ƽ��Ա\n");
		break;
    }
	printf("�ֻ���: %s\n\n", currentUser->phonenumber);
    printf("1. �޸�����\n");
	printf("2. ����\n");
    printf("��ѡ�������");
    scanf("%d", &choice);
    switch (choice) {
    case 1:
		changePassword();
		break;
    case 2:
		break;
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

    AddUserCheck();
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
            running = 0;
            break;
        default:
            printf("��Чѡ�����������룡\n");
            waitForKeyPress();
        }
    }
    showAdminMenu();
}


// ��ʾ�����û�
void displayAllUsers() {
    clearScreen();
    printf("=================================\n");
    printf("           �����û�             \n");
    printf("=================================\n");
    printf("%-5s %-20s %-15s %-15s %-20s\n", "ID", "�û���", "��Ա�ȼ�", "��������","�ֻ���");
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

            printf("%-15.2f", current->consumptionLevel);
			printf("%-20s\n", current->phonenumber);
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
    AddUserCheck();
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
            running = 0;
            break;
        default:
            printf("��Чѡ�����������룡\n");
            waitForKeyPress();
        }
    }
    showAdminMenu();
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
    int choice;
    clearScreen();
    printf("=================================\n");
    printf("           ��Ӱ���             \n");
    printf("=================================\n");

    int userId;
    printf("ѡ�����뷽ʽ��\n");
    printf("1.�û�ID\n");
    printf("2.�û��ֻ���\n");
    printf("3.�û���\n");
	printf("��ѡ�������");
    
	scanf("%d", &choice);
	if (choice == 2) {
		char phonenumber[50];
		printf("�ռ����ֻ���: ");
		scanf("%s", phonenumber);
		User* user = findUserByPhone(phonenumber);
		if (user == NULL) {
			printf("�û������ڣ�\n");
			waitForKeyPress();
			return;
		}
		userId = user->id;
	}
	if (choice == 1) {
        printf("�ռ����û�ID: ");
        scanf("%d", &userId);
		User* user = findUserById(userId);
		if (user == NULL) {
			printf("�û������ڣ�\n");
			waitForKeyPress();
			return;
		}
        userId = user->id;
	}
	if (choice == 3) {
		char username[506];
		printf("�ռ����û���: ");
		scanf("%s", username);
		User* user = findUserByUsername(username);
		if (user == NULL) {
			printf("�û������ڣ�\n");
			waitForKeyPress();
			return;
		}
		userId = user->id;
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
        savePackages_File("packages.txt");
        saveShelvesToFile("shelves.txt");

        // ��ӼƼ��������¼
        double fee = calculatePackageFee(size, weight, transportMethod);
        add_Transaction(TRANSACTION_INCOME, INCOME_PIECE_FEE, fee, "�����Ƽ���");
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
            running = 0;
            break;
        default:
            printf("��Чѡ�����������룡\n");
            waitForKeyPress();
        }
    }
    showAdminMenu();
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
    // ��ʾ���˵�
    while (running) {
        showMainMenu();
        printf("��ѡ�������");
        int choice;
        if (scanf("%d", &choice) != 1) { // ���scanf�Ƿ�ɹ���ȡ��һ������
            clearInputBuffer();
            printf("���뺬�з����֣�����������ѡ�\n");
            waitForKeyPress();
            continue;
        }


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
            printf("������Ч������������0��1��2��\n");
            waitForKeyPress();
			clearScreen();
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


// ����ȡ��
void handlePickupPackage() {
    clearScreen();
    printf("=================================\n");
    printf("             ȡ��               \n");
    printf("=================================\n");

    int packageInput;
    int choice;
    double price;
    int status;
    printf("ѡ����ҷ�ʽ��\n1.����ID\n2.ȡ����\n");
    scanf("%d", &choice);
    if (choice == 1) {
        printf("���������ID: ");
        scanf("%d", &packageInput);
        Package* package = findPackageById(packageInput);
        if (package == NULL) {
            printf("δ�ҵ�������\n");
        }
		if (package->status == PACKAGE_STATUS_PICKED) {
			printf("������֮ǰ�ѱ�ȡ����\n");
			waitForKeyPress();
			return;
		}
        if (markPackageAsPickedUp(packageInput)) {
            printf("�����ѳɹ�ȡ����\n");
            savePackages_File("packages.txt");
            saveShelvesToFile("shelves.txt");

        }
        else {
            printf("����ʧ�ܣ�\n");
        }
		waitForKeyPress();
    }
    if (choice == 2) {
        char pickupCode[20];
        printf("������ȡ����: ");
        scanf("%s", pickupCode);
        Package* package = findPackageByCode(pickupCode);
        if (package == NULL) {
            printf("δ�ҵ�������\n");
        }
        if(package->status == PACKAGE_STATUS_PICKED) {
            printf("������֮ǰ�ѱ�ȡ����\n");
            waitForKeyPress();
            return;
        }
        else {
            if (markPackageAsPickedUp(package->id)) {
                printf("�����ѳɹ�ȡ����\n");
                savePackages_File("packages.txt");
                saveShelvesToFile("shelves.txt");
            }
            else {
                printf("����ʧ�ܣ��������ܲ����ڻ��Ѿ�ȡ����\n");
            }
        }
        waitForKeyPress();
    }
}