#ifndef STATISTICS_H
#define STATISTICS_H
#include "main.h"

// �����ձ�
void generateDailyReport(const char* date, char* reportOutput);

// �����ܱ�
void generateWeeklyReport(const char* startDate, const char* endDate, char* reportOutput);

// �����±�
void generateMonthlyReport(const char* month, char* reportOutput);

// ������������
void analyzePackageFlow(char* analysisOutput);

// �������
void analyzeIncome(char* analysisOutput);

// Ѱ�����Ż��ܴ��
void optimizeShelfPlacement(char* optimizationOutput);



// ���������ֲ�
void analyzePackageDistribution(char* output);

// �����û�����ģʽ
void analyzeUserConsumptionPattern(int userId, char* output);

// Ԥ��δ��������
void predictFuturePackageVolume(int days, char* output);

// ��������������
void analyzeShelfUtilization(char* output);

// ���ɽ����Ż�����
void generateOptimizationSuggestions(char* output);

// ����������������λ�õĹ�ϵ
void analyzePackageShelfCorrelation(char* output);

// ����ͼ�����ݣ�JSON��ʽ�����ں��漸�ܿ��ܵ�Qt��ʾ��
void generateChartData(int chartType, char* jsonOutput);

void optimizeShelfPlacement(char* optimizationOutput);

#endif /* STATISTICS_H */