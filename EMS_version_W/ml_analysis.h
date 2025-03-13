#ifndef ML_ANALYSIS_H
#define ML_ANALYSIS_H

#include "main.h"

#define MAX_FEATURES 10
#define MAX_SAMPLES 1000

typedef struct {
    float features[MAX_FEATURES];
    float label;
} TrainingSample;

typedef struct {
    float weights[MAX_FEATURES];
    float bias;
    int feature_count;
} Model;

// 初始化机器学习模型
void init_ml_model(Model* model);

// 训练线性回归模型
void train_linear_regression(Model* model, TrainingSample* samples, int sample_count);

// 预测货架周转率
float predict_turnover_rate(Model* model, Shelf* shelf);

// 保存模型到文件
void save_model(Model* model, const char* filename);

// 从文件加载模型
void load_model(Model* model, const char* filename);

// 特征工程：提取货架周转率特征
void extract_turnover_features(Shelf* shelf, float* features);

// 特征工程：提取商品关联性特征
void extract_association_features(Shelf* shelf, float* features);


extern Model shelf_model;
#endif /* ML_ANALYSIS_H */