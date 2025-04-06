#ifndef PRICING_H
#define PRICING_H

#include "main.h"

// 计算会员折扣
double calculateMemberDiscount(int memberLevel);

// 计算大数据杀熟加价
double calculatePriceBump(int userId, double basePrice);

// 计算最终价格
double calculateFinalPrice(int userId, double basePrice);



// 计算赔偿金额
double calculateCompensationAmount(Package* package);

// 更新用户会员等级
int updateUserMemberLevel(int userId);

#endif /* PRICING_H */
