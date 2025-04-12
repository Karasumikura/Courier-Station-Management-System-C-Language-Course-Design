#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>  // Windows ƽ̨ʹ��
#else
#include <unistd.h>   // Linux/Unix ƽ̨ʹ��
#endif
#include "main.h"
#include "util.h"
#include "user.h"

int Promotionstatus;
static int lastDay = -1; 
static int counter = 0;

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

void timecheck() {
    struct tm start_time = { 0 }, end_time = { 0 };
    User* currentUser = findUserById(g_currentUserId);
    start_time.tm_year = 2025 - 1900;
    start_time.tm_mon = 2;//���·�0��ʼ��
    start_time.tm_mday = 21;
    start_time.tm_hour = 0;
    start_time.tm_min = 0;
    start_time.tm_sec = 0;

    end_time.tm_year = 2025 - 1900;
    end_time.tm_mon = 5;
    end_time.tm_mday = 21;
    end_time.tm_hour = 23;
    end_time.tm_min = 59;
    end_time.tm_sec = 59;

    time_t start_timestamp = mktime(&start_time);
    time_t end_timestamp = mktime(&end_time);

    time_t current_time = time(NULL);

    if (current_time >= start_timestamp && current_time <= end_timestamp)
    {
        Promotionstatus = 1;
        currentUser->Promotionstatus_user = 1;
    }
    else {
        Promotionstatus = 0;
        currentUser->Promotionstatus_user = 0;
    }

}

int generateUniqueId() {
    
    srand((unsigned int)time(NULL) + rand());
    return rand() % 9000 + 1000; 
}

void getCurrentTimeString(char* timeStr) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);

    sprintf(timeStr, "%04d-%02d-%02d %02d:%02d:%02d",
        tm_info->tm_year + 1900, tm_info->tm_mon + 1, tm_info->tm_mday,
        tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
}
//strftime ��һ�����ڸ�ʽ�����ں�ʱ��ĺ�����
// ������ָ���ĸ�ʽ�ַ����� struct tm ���͵�ʱ����Ϣת��Ϊ�ַ�����ʽ
void gettimeonlyday(char *dateStr) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(dateStr, 11, "%Y-%m-%d", tm_info);
}

char* timeinput() {
    char timeStr[40];
    getchar();
    printf("������ʱ�䣨��ʽ��YYYY-MM-DD HH:MM:SS����");
    if (fgets(timeStr, sizeof(timeStr), stdin) == NULL) {
        printf("��ȡ����ʧ�ܣ�\n");
        waitForKeyPress();
        return NULL;
    }


    size_t len = strlen(timeStr);
    if (len > 0 && timeStr[len - 1] == '\n') {
        timeStr[len - 1] = '\0';
    }


    if (!isValidDateFormat(timeStr)) {
        printf("���ڸ�ʽ��Ч��\n");
        waitForKeyPress();
        return NULL;
    }
    return _strdup(timeStr);
}

int isValidDateFormat(const char* dateStr) {
    int year, month, day, hour, minute, second;
    if (sscanf(dateStr, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) != 6) {
        return 0; 
    }

   
    struct tm tm_date = { 0 };
    tm_date.tm_year = year - 1900;
    tm_date.tm_mon = month - 1;
    tm_date.tm_mday = day;
    tm_date.tm_hour = hour;
    tm_date.tm_min = minute;
    tm_date.tm_sec = second;

   
    if (mktime(&tm_date) == -1) {
        return 0; 
    }

    return 1;
}

char* getNexttime(const char* dateStr,int time) {
    struct tm tm_date = { 0 };
    time_t raw_time;

    
    if (sscanf(dateStr, "%d-%d-%d", &tm_date.tm_year, &tm_date.tm_mon, &tm_date.tm_mday) != 3) {
        return NULL; 
    }

    
    tm_date.tm_year -= 1900;
    tm_date.tm_mon -= 1;

    
    raw_time = mktime(&tm_date);
    if (raw_time == -1) {
        return NULL; 
    }
    raw_time += 24 * 60 * 60 * time; 

    
    struct tm result;
    if (_localtime64_s(&result, (__time64_t*)&raw_time) != 0) {
        return NULL; 
    }

    
    char* nextDay = (char*)malloc(20 * sizeof(char));
    if (nextDay == NULL) {
        return NULL;
    }
    sprintf(nextDay, "%04d-%02d-%02d",
        result.tm_year + 1900,
        result.tm_mon + 1,
        result.tm_mday);

    return nextDay;
}

void initializedateFile() {
    FILE* file = fopen("counter_data.txt", "w");
    if (file == NULL) {
        printf("�޷������ļ���\n");
        exit(1);
    }
    char currentDate[11];
    gettimeonlyday(currentDate);
    fprintf(file, "%s\n1\n", currentDate); 
    fclose(file);
}

int readCounterData(char* lastDate, int* counter) {
    FILE* file = fopen("counter_data.txt", "r");
    if (file == NULL) {
        return 0; 
    }

    
    if (fscanf(file, "%10s\n%d", lastDate, counter) != 2) {
        fclose(file);
        return 0; 
    }

    fclose(file);
    return 1; 
}


void writeCounterData(const char* currentDate, int counter) {
    FILE* file = fopen("counter_data.txt", "w");
    if (file == NULL) {
        printf("�޷�д���ļ���\n");
        exit(1);
    }
    fprintf(file, "%s\n%d\n", currentDate, counter); 
    fclose(file);
}


int getDailyIncrementalNumber() {
    char currentDate[11];
    char lastDate[11];
    int counter;

   
    gettimeonlyday(currentDate);

    
    if (!readCounterData(lastDate, &counter)) {
        // ����ļ������ڻ��ȡʧ�ܣ���ô�ͳ�ʼ���ļ�
        initializedateFile();
        strcpy(lastDate, currentDate);
        counter = 1;
    }
    else {
        
        if (strcmp(currentDate, lastDate) != 0) {
          
            counter = 1;
        }
        else {
           
            counter++;
        }
    }

    
    writeCounterData(currentDate, counter);

    return counter;
}

void set_cursor_visibility(int visible) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);  // ��ȡ��ǰ�����Ϣ
    cursorInfo.bVisible = visible;               // ���ù���Ƿ�ɼ�
    SetConsoleCursorInfo(hConsole, &cursorInfo); // Ӧ���µĹ����Ϣ
}

void loading_simulation() {
#ifdef _WIN32
    // ���ع�꣨Win��
    set_cursor_visibility(0);
#else
    // ���ع�꣨Linux��
    printf(HIDE_CURSOR);
    fflush(stdout);
#endif
    for (int i = 0; i < WAITING_TIME; i++) {
        switch (i % 3) {
        case 0:
            printf("\r������.  ");  // \r �ص�����
            break;
        case 1:
            printf("\r������.. ");
            break;
        case 2:
            printf("\r������...");
            break;
        }
        fflush(stdout);  // ȷ���������������̨
        Sleep(100);
    }
    printf("\r         \r");
#ifdef _WIN32
    // �ָ���꣨Windows ƽ̨��
    set_cursor_visibility(1);
#else
    // �ָ���꣨Linux/Unix ƽ̨��
    printf(SHOW_CURSOR);
    fflush(stdout);
#endif
}

int isValidMonthFormat(const char* month) {
    // ����ַ��������Ƿ�Ϊ 7
    if (strlen(month) != 7) {
        return 0; // ���Ȳ�����Ҫ��
    }

    // ���ǰ 4 ���ַ��Ƿ�Ϊ����
    for (int i = 0; i < 4; i++) {
        if (!isdigit(month[i])) {
            return 0; // ��ݲ��ְ����������ַ�
        }
    }

    // ���� 5 ���ַ��Ƿ�Ϊ '-'
    if (month[4] != '-') {
        return 0; // �ָ�������
    }

    for (int i = 5; i < 7; i++) {
        if (!isdigit(month[i])) {
            return 0; // �·ݲ��ְ����������ַ�
        }
    }

    // ��ȡ�·ݲ���֤��Χ
    int monthValue = atoi(month + 5); // ��ȡ�·ݲ���
    if (monthValue < 1 || monthValue > 12) {
        return 0; // �·ݲ�����Ч��Χ��
    }

    return 1; // ��ʽ��ȷ
}

void linearRegression(int n, double x[], double y[], double* a, double* b) {
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;

    for (int i = 0; i < n; i++) {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_x2 += x[i] * x[i];
    }

    *a = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
    *b = (sum_y - (*a) * sum_x) / n;
}//���������Իع�ĺ���

int dataprepocessing(Record records[]) {
    FILE* file = fopen("transactions.txt", "r");
    if (file == NULL) {
        printf("�޷����ļ� transactions.txt��\n");
        return 0;
    }

    int count = 0;//��������
    char line[256];
    while (fgets(line, sizeof(line), file)) {
		char* token = strtok(line, ",");//strtok����python��.split,����ֻ�洢��һ���ָ���ַ���
        int index = 0;//��¼�������ֱ���������
        while (token != NULL && index < 5) {
            if (index == 1) {
				records[count].status = atoi(token);//����or֧��
                //atoi,atof���ַ���ת��Ϊint,double
            }
            else if (index == 2) {//�Ƿ����Ϊ����
                if (atoi(token) == INCOME_PIECE_FEE) {
					records[count].ifnewpackage = 1;
                }
                else records[count].ifnewpackage = 0;
            }
            else if (index == 3) { // ��ȡ�۸�
                records[count].price = atof(token);
            }
            else if (index == 4) { // ��ȡʱ��
                strncpy(records[count].timestamp, token, 19);
                records[count].timestamp[19] = '\0';
            }
			token = strtok(NULL, ",");//�����ָ�
            index++;
        }
        count++;
    }
	if (count > MAX_RECORDS) {
		printf("���ݳ���������ƣ��޷�����\n");
		waitForKeyPress();
		fclose(file);
		return -1;
	}
    fclose(file);
    return count;
}//���ݹ��̺���

int daysummary(Record records[], int recordCount, DailySummary summaries[]) {
    int summaryCount = 0;

    for (int i = 0; i < recordCount; i++) {
        char currentDate[11];
        strncpy(currentDate, records[i].timestamp, 10);//��ȡʱ�䵽��
        currentDate[10] = '\0';

        int found = 0;
        for (int j = 0; j < summaryCount; j++) {
            if (strcmp(summaries[j].date, currentDate) == 0) {//���ҵ���Ӧʱ��
				if (records[i].status == TRANSACTION_INCOME) {
					summaries[j].totalincome += records[i].price;
				}
				else {
					summaries[j].totaloutcome += records[i].price;
				}
                found = 1;
                if (records[i].ifnewpackage == 1) {
                    summaries[summaryCount].totalPackages++;
                }
                break;
            }
        }

        if (!found) {
            strncpy(summaries[summaryCount].date, currentDate, 10);//�½�һ����Ӧʱ��
            summaries[summaryCount].date[10] = '\0';
            if (records[i].status == TRANSACTION_INCOME) {
                summaries[summaryCount].totalincome = records[i].price;
				summaries[summaryCount].totaloutcome = 0;
            }
            else {
                summaries[summaryCount].totaloutcome = records[i].price;
                summaries[summaryCount].totalincome = 0;
            }
			if (records[i].ifnewpackage == 1) {
				summaries[summaryCount].totalPackages = 1;
			}
			else {
				summaries[summaryCount].totalPackages = 0;
			}
            summaryCount++;
        }
    }

    return summaryCount;
}

void predictFuture(int summaryCount, DailySummary summaries[]) {
    double x[MAX_RECORDS], y1[MAX_RECORDS],y2[MAX_RECORDS],y3[MAX_RECORDS];

    for (int i = 0; i < summaryCount; i++) {
        x[i] = i + 1; // ʱ�����
        y1[i] = summaries[i].totalincome;
		y2[i] = summaries[i].totaloutcome;
		y3[i] = summaries[i].totalPackages;
    }

    double a1, b1,a2,b2,a3,b3;
    linearRegression(summaryCount, x, y1, &a1, &b1);
	linearRegression(summaryCount, x, y2, &a2, &b2);
	linearRegression(summaryCount, x, y3, &a3, &b3);
    printf("\n�����Զ�������\n");
    printf("�������Իع鷽�̣�y = %.2fx + %.2f\n", a1, b1);
	printf("֧�����Իع鷽�̣�y = %.2fx + %.2f\n", a2, b2);
	printf("�����������Իع鷽�̣�y = %.2fx + %.2f\n", a3, b3);

    int futureDays;
	printf("������Ԥ��������");
	if (scanf("%d", &futureDays) != 1 || futureDays <= 0) {
		printf("��Ч�����룡\n");
		waitForKeyPress();
		return;
	}
    printf("\n=================================\n");
    printf("          Ԥ��δ�� %d ��", futureDays);
    printf("\n=================================\n");
    for (int i = 0; i < futureDays; i++) {
        double predicted_y1 = a1 * (summaryCount + i + 1) + b1;
		double predicted_y2 = a2 * (summaryCount + i + 1) + b2;
		double predicted_y3 = a3 * (summaryCount + i + 1) + b3;
		if (predicted_y2 > 0.0) predicted_y2 = 0.0;
		if (predicted_y1 < 0.0) predicted_y1 = 0.0;
        if ((predicted_y3 - (int)predicted_y3) >= 0.5) {
            predicted_y3 = (int)predicted_y3 + 1;
		}
		else {
			predicted_y3 = (int)predicted_y3;
		}
        printf("δ���� %d �죺�����룺%.2lfԪ����֧����%.2lfԪ���ܰ�����������%d��\n", 
            i + 1, predicted_y1,predicted_y2, predicted_y3);
    }
    waitForKeyPress();
}