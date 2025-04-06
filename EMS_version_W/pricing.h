#ifndef PRICING_H
#define PRICING_H

#include "main.h"

// �����Ա�ۿ�
double calculateMemberDiscount(int memberLevel);

// ���������ɱ��Ӽ�
double calculatePriceBump(int userId, double basePrice);

// �������ռ۸�
double calculateFinalPrice(int userId, double basePrice);



// �����⳥���
double calculateCompensationAmount(Package* package);

// �����û���Ա�ȼ�
int updateUserMemberLevel(int userId);

#endif /* PRICING_H */
