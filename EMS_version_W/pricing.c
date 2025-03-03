#include <stdio.h>
#include "main.h"
#include "pricing.h"
#include "user.h"

// �����Ա�ۿ�
double calculateMemberDiscount(int memberLevel) {
    switch (memberLevel) {
    case USER_NEW:
        return 0.05; // ���û�5%�ۿ�
    case USER_SILVER:
        return 0.1;  // ������Ա10%�ۿ�
    case USER_GOLD:
        return 0.2;  // �ƽ��Ա20%�ۿ�
    default:
        return 0.0;
    }
}

// ���������ɱ��Ӽ�
double calculatePriceBump(int userId, double basePrice) {
    User* user = findUserById(userId);
    if (user == NULL) {
        return 0.0;
    }

    // �����û����������ּ��Ӽ�
    if (user->consumptionLevel > 1000) {
        return basePrice * 0.1; // �����������û��Ӽ�10%
    }
    else if (user->consumptionLevel > 500) {
        return basePrice * 0.05; // �����������û��Ӽ�5%
    }

    return 0.0;
}

// �������ռ۸�
double calculateFinalPrice(int userId, double basePrice) {
    User* user = findUserById(userId);
    if (user == NULL) {
        return basePrice;
    }

    // Ӧ�û�Ա�ۿ�
    double discount = calculateMemberDiscount(user->memberLevel);
    double discountAmount = basePrice * discount;

    // Ӧ�ô�����ɱ��Ӽ�
    double priceBump = calculatePriceBump(userId, basePrice);

    // ���ռ۸� = �����۸� - �ۿ� + �Ӽ�
    return basePrice - discountAmount + priceBump;
}