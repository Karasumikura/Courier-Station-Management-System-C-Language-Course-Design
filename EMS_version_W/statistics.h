#ifndef STATISTICS_H
#define STATISTICS_H
#include "main.h"

// 报告生成
void generateDailyReport(char* reportOutput);


void generateWeeklyReport(const char* startDate, const char* endDate, char* reportOutput);


void generateMonthlyReport(const char* month, char* reportOutput);


void analyzePackageFlow(char* analysisOutput);


void analyzeIncome(char* analysisOutput);

void optimizeShelfPlacement(char* optimizationOutput);


#endif /* STATISTICS_H */