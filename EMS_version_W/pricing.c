#include <stdio.h>
#include "main.h"
#include "pricing.h"
#include "user.h"
#include "statistics.h"
#include "util.h"
//会员系统
double calculateMemberDiscount(int memberLevel) {
    switch (memberLevel) {
    case USER_NEW:
        return 0.05; 
    case USER_SILVER:
        return 0.1;  
    case USER_GOLD:
        return 0.2;  
    default:
        return 0.0;
    }
}

//通过大数据杀熟，根据用户不同的消费能力加价
double calculatePriceBump(int userId, double basePrice) {
    User* user = findUserById(userId);
    if (user == NULL) {
        return 0.0;
    }

    
    if (user->consumptionLevel > 1000) {
        return basePrice * 0.22; 
    }
    else if (user->consumptionLevel > 500) {
        return basePrice * 0.11; 
    }

    return 0.0;
}


double calculateFinalPrice(int userId, double basePrice) {
    User* user = findUserById(userId);
    if (user == NULL) {
        return basePrice;
    }
    if (Promotionstatus) {
        timecheck;//用户领了券才查过期了没，如果他没领不会主动发折扣券，领了要保证不被多薅羊毛，主打一个省钱-
    }
    
    double discount = calculateMemberDiscount(user->memberLevel);
    double discountAmount;
    if (Promotionstatus)
    {
        discountAmount = basePrice * discount;
    }
    else {
        discountAmount = 0;
    }
    

    
    double priceBump = calculatePriceBump(userId, basePrice);

    
    return basePrice - discountAmount + priceBump;
}