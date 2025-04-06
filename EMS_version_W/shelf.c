#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "shelf.h"
#include "util.h"

// ������Ϣϵͳ
Shelf* g_shelfList = NULL;


void initShelfList() {
    g_shelfList = NULL;
}


Shelf* addShelf(int type, int capacity) {
    
    Shelf* newShelf = (Shelf*)malloc(sizeof(Shelf));
    if (newShelf == NULL) {
        return NULL;
    }

    
    newShelf->id = generateUniqueId();
    newShelf->type = type;
    newShelf->capacity = capacity;
    newShelf->currentCount = 0;

    
    newShelf->next = g_shelfList;
    g_shelfList = newShelf;

    return newShelf;
}


Shelf* findShelfById(int shelfId) {
    Shelf* current = g_shelfList;

    while (current != NULL) {
        if (current->id == shelfId) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}


int findSuitableShelf(int packageSize, int packageNote) {
    Shelf* current = g_shelfList;
    Shelf* bestShelf = NULL;

    if (packageNote == PACKAGE_NOTE_FRAGILE) {
        
        while (current != NULL) {
            if (current->type == 5 && current->currentCount < current->capacity) {
                return current->id;
            }
            current = current->next;
        }
        // ���û���ҵ������˵�ʹ�óߴ�ƥ�����ͨ����
        current = g_shelfList;
    }
    else if (packageNote == PACKAGE_NOTE_FRESH) {
        
        while (current != NULL) {
            if (current->type == 6 && current->currentCount < current->capacity) {
                return current->id;
            }
            current = current->next;
        }
        // ���ʱ���������ʻ����ϣ����û�пռ��򷵻�ʧ��
        return -1;
    }

    
    while (current != NULL) {
        if (current->type == packageSize && current->currentCount < current->capacity) {
            
            return current->id;
        }
        else if (current->type > packageSize && current->currentCount < current->capacity) {
            // �ϴ�Ļ���Ҳ����ʹ�ã���¼���ƥ��
            if (bestShelf == NULL || current->type < bestShelf->type) {
                bestShelf = current;
            }
        }
        current = current->next;
    }

    return bestShelf != NULL ? bestShelf->id : -1;
}


void updateShelfCount(int shelfId, int delta) {
    Shelf* shelf = findShelfById(shelfId);
    if (shelf != NULL) {
        shelf->currentCount += delta;

      
        if (shelf->currentCount < 0) {
            shelf->currentCount = 0;
        }

        
        if (shelf->currentCount > shelf->capacity) {
            shelf->currentCount = shelf->capacity;
        }
    }
}

float getShelfUtilization(int shelfId) {
    Shelf* shelf = findShelfById(shelfId);
    if (shelf == NULL) {
        return 0.0f;
    }

    return (float)shelf->currentCount / shelf->capacity;
}


void getShelvesStatus(char* statusReport) {
    char buffer[1024] = "";
    Shelf* current = g_shelfList;

    while (current != NULL) {
        char shelfInfo[100];
        float utilization = (float)current->currentCount / current->capacity * 100;

        sprintf(shelfInfo, "����ID: %d, ����: %d, ʹ����: %.1f%%\n",
            current->id, current->type, utilization);

        strcat(buffer, shelfInfo);
        current = current->next;
    }

    strcpy(statusReport, buffer);
}

// �����������ݵ��ļ�
void saveShelvesToFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("�޷��򿪻��������ļ� %s\n", filename);
        return;
    }

    Shelf* current = g_shelfList;
    while (current != NULL) {
        fprintf(file, "%d,%d,%d,%d\n",
            current->id,
            current->type,
            current->capacity,
            current->currentCount);
        current = current->next;
    }

    fclose(file);
}

//��ȡ�ļ��еĻ�������
void loadShelvesFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("���������ļ� %s �����ڣ����������ļ�\n", filename);
        return;
    }

    
    while (g_shelfList != NULL) {
        Shelf* temp = g_shelfList;
        g_shelfList = g_shelfList->next;
        free(temp);
    }

    
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        int id, type, capacity, currentCount;

        if (sscanf(line, "%d,%d,%d,%d", &id, &type, &capacity, &currentCount) == 4) {
           
            Shelf* newShelf = (Shelf*)malloc(sizeof(Shelf));
            if (newShelf != NULL) {
                newShelf->id = id;
                newShelf->type = type;
                newShelf->capacity = capacity;
                newShelf->currentCount = currentCount;

                  
                newShelf->next = g_shelfList;
                g_shelfList = newShelf;
            }
        }
    }

    fclose(file);
}

void checkeveryshelf() {
    int status = 0;
    float utilization = 0.0;
    Shelf* current = g_shelfList;
    while (current != NULL) {
        utilization = getShelfUtilization(current->id);
        if (utilization > 0.8) {
            printf("%d ", current->id);
        }
        current = current->next;
    }
	if (!status) {
		printf("��\n");
	}
	else {
		printf("����ʹ���ʳ���80%%\n");
	}
}