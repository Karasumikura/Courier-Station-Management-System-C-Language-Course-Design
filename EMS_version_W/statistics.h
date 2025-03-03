#ifndef STATISTICS_H
#define STATISTICS_H

#include "main.h"

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

#endif /* STATISTICS_H */