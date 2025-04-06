#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "shelf.h"
#include "util.h"

// 货架信息系统
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
        // 如果没有找到，回退到使用尺寸匹配的普通货架
        current = g_shelfList;
    }
    else if (packageNote == PACKAGE_NOTE_FRESH) {
        
        while (current != NULL) {
            if (current->type == 6 && current->currentCount < current->capacity) {
                return current->id;
            }
            current = current->next;
        }
        // 冷鲜必须放在冷鲜货架上，如果没有空间则返回失败
        return -1;
    }

    
    while (current != NULL) {
        if (current->type == packageSize && current->currentCount < current->capacity) {
            
            return current->id;
        }
        else if (current->type > packageSize && current->currentCount < current->capacity) {
            // 较大的货架也可以使用，记录最佳匹配
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

        sprintf(shelfInfo, "货架ID: %d, 类型: %d, 使用率: %.1f%%\n",
            current->id, current->type, utilization);

        strcat(buffer, shelfInfo);
        current = current->next;
    }

    strcpy(statusReport, buffer);
}

// 导出货架数据到文件
void saveShelvesToFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法打开货架数据文件 %s\n", filename);
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

//读取文件中的货架数据
void loadShelvesFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("货架数据文件 %s 不存在，将创建新文件\n", filename);
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
		printf("无\n");
	}
	else {
		printf("货架使用率超过80%%\n");
	}
}