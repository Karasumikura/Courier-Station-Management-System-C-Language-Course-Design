#include "ml_analysis.h"
#include "shelf.h"
#include <math.h>

void init_ml_model(Model* model) {
    memset(model->weights, 0, sizeof(float)*MAX_FEATURES);
    model->bias = 0.0f;
    model->feature_count = MAX_FEATURES;
}

void train_linear_regression(Model* model, TrainingSample* samples, int sample_count) {
    // 简单实现梯度下降算法
    float learning_rate = 0.01f;
    for(int epoch=0; epoch<100; epoch++){
        for(int i=0; i<sample_count; i++){
            float prediction = model->bias;
            for(int j=0; j<model->feature_count; j++){
                prediction += model->weights[j] * samples[i].features[j];
            }
            float error = prediction - samples[i].label;
            
            // 更新参数
            model->bias -= learning_rate * error;
            for(int j=0; j<model->feature_count; j++){
                model->weights[j] -= learning_rate * error * samples[i].features[j];
            }
        }
    }
}

float predict_turnover_rate(Model* model, Shelf* shelf) {
    float features[MAX_FEATURES];
    extract_turnover_features(shelf, features);
    
    float prediction = model->bias;
    for(int i=0; i<model->feature_count; i++){
        prediction += model->weights[i] * features[i];
    }
    return prediction;
}

void save_model(Model* model, const char* filename) {
    FILE* fp = fopen(filename, "wb");
    if(fp) {
        fwrite(model->weights, sizeof(float), MAX_FEATURES, fp);
        fwrite(&model->bias, sizeof(float), 1, fp);
        fclose(fp);
    }
}

void load_model(Model* model, const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if(fp) {
        fread(model->weights, sizeof(float), MAX_FEATURES, fp);
        fread(&model->bias, sizeof(float), 1, fp);
        fclose(fp);
    }
}

void extract_turnover_features(Shelf* shelf, float* features) {
    features[0] = shelf->currentCount / (float)shelf->capacity;
    features[1] = shelf->turnover_count / 100.0f;
    features[2] = shelf->last_restock_days;
}

void extract_association_features(Shelf* shelf, float* features) {
    // 实现商品关联性特征提取
    features[3] = shelf->related_items_count / 10.0f;
    features[4] = shelf->category_diversity;
    features[5] = (time(NULL) - shelf->last_category_change) / 86400.0f;
}
