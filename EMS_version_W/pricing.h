#ifndef PRICING_H
#define PRICING_H

#include "main.h"
//��Աϵͳ

double calculateMemberDiscount(int memberLevel);


double calculatePriceBump(int userId, double basePrice);


double calculateFinalPrice(int userId, double basePrice);



double calculateCompensationAmount(Package* package);


int updateUserMemberLevel(int userId);

#endif /* PRICING_H */
