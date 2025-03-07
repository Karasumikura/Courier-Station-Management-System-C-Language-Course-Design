#ifndef STATISTICS_H
#define STATISTICS_H
#include "main.h"

// 生成日报
void generateDailyReport(const char* date, char* reportOutput);

// 生成周报
void generateWeeklyReport(const char* startDate, const char* endDate, char* reportOutput);

// 生成月报
void generateMonthlyReport(const char* month, char* reportOutput);

// 包裹流量分析
void analyzePackageFlow(char* analysisOutput);

// 收入分析
void analyzeIncome(char* analysisOutput);

// 寻找最优货架存放
void optimizeShelfPlacement(char* optimizationOutput);



// 分析包裹分布
void analyzePackageDistribution(char* output);

// 分析用户消费模式
void analyzeUserConsumptionPattern(int userId, char* output);

// 预测未来包裹量
void predictFuturePackageVolume(int days, char* output);

// 分析货架利用率
void analyzeShelfUtilization(char* output);

// 生成建议优化方案
void generateOptimizationSuggestions(char* output);

// 分析包裹特性与存放位置的关系
void analyzePackageShelfCorrelation(char* output);

// 生成图表数据（JSON格式，用于后面几周可能的Qt显示）
void generateChartData(int chartType, char* jsonOutput);

void optimizeShelfPlacement(char* optimizationOutput);

#endif /* STATISTICS_H */