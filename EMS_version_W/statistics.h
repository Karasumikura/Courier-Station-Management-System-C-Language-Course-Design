#ifndef STATISTICS_H
#define STATISTICS_H

#include "main.h"

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

#endif /* STATISTICS_H */