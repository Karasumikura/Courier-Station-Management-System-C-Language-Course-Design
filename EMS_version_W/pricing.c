#include <stdio.h>
#include "main.h"
#include "pricing.h"
#include "user.h"
#include "statistics.h"
#include "util.h"
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
        return basePrice * 0.22; // �����������û��Ӽ�22%
    }
    else if (user->consumptionLevel > 500) {
        return basePrice * 0.11; // �����������û��Ӽ�11%
    }

    return 0.0;
}

// �������ռ۸�
double calculateFinalPrice(int userId, double basePrice) {
    User* user = findUserById(userId);
    if (user == NULL) {
        return basePrice;
    }
    if (Promotionstatus) {
        timecheck;//�û�����ȯ�Ų������û�������û�첻���������ۿ�ȯ������Ҫ��֤������޶��ë������һ��ʡǮ
    }
    // Ӧ�û�Ա�ۿ�
    double discount = calculateMemberDiscount(user->memberLevel);
    double discountAmount;
    if (Promotionstatus)
    {
        discountAmount = basePrice * discount;
    }
    else {
        discountAmount = 0;
    }
    

    // Ӧ�ô�����ɱ��Ӽ�
    double priceBump = calculatePriceBump(userId, basePrice);

    // ���ռ۸� = �����۸� - �ۿ� + �Ӽ�
    return basePrice - discountAmount + priceBump;
}