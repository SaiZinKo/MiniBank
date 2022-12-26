//
// Created by zinko on 12/25/2022.
//

#include "random"
#include "common-utils.h"

int generateId() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100000);
    return dis(gen);
}
