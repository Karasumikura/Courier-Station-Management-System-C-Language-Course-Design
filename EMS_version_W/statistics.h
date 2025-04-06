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



void analyzePackageDistribution(char* output);

void analyzeUserConsumptionPattern(int userId, char* output);


void predictFuturePackageVolume(int days, char* output);


void analyzeShelfUtilization(char* output);


void generateOptimizationSuggestions(char* output);

void analyzePackageShelfCorrelation(char* output);

// 生成图表数据（JSON格式，用于后面几周可能的Qt显示）
void generateChartData(int chartType, char* jsonOutput);

void optimizeShelfPlacement(char* optimizationOutput);

#endif /* STATISTICS_H */