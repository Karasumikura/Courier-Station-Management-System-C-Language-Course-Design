#include <stdio.h>
#include "main.h"
#include "pricing.h"
#include "user.h"
#include "statistics.h"
#include "util.h"
// 计算会员折扣
double calculateMemberDiscount(int memberLevel) {
    switch (memberLevel) {
    case USER_NEW:
        return 0.05; // 新用户5%折扣
    case USER_SILVER:
        return 0.1;  // 白银会员10%折扣
    case USER_GOLD:
        return 0.2;  // 黄金会员20%折扣
    default:
        return 0.0;
    }
}

// 计算大数据杀熟加价
double calculatePriceBump(int userId, double basePrice) {
    User* user = findUserById(userId);
    if (user == NULL) {
        return 0.0;
    }

    // 根据用户消费能力分级加价
    if (user->consumptionLevel > 1000) {
        return basePrice * 0.22; // 高消费能力用户加价22%
    }
    else if (user->consumptionLevel > 500) {
        return basePrice * 0.11; // 中消费能力用户加价11%
    }

    return 0.0;
}

// 计算最终价格
double calculateFinalPrice(int userId, double basePrice) {
    User* user = findUserById(userId);
    if (user == NULL) {
        return basePrice;
    }
    if (Promotionstatus) {
        timecheck;//用户领了券才查过期了没，如果他没领不会主动发折扣券，领了要保证不被多薅羊毛，主打一个省钱
    }
    // 应用会员折扣
    double discount = calculateMemberDiscount(user->memberLevel);
    double discountAmount;
    if (Promotionstatus)
    {
        discountAmount = basePrice * discount;
    }
    else {
        discountAmount = 0;
    }
    

    // 应用大数据杀熟加价
    double priceBump = calculatePriceBump(userId, basePrice);

    // 最终价格 = 基础价格 - 折扣 + 加价
    return basePrice - discountAmount + priceBump;
}