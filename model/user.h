//
// Created by Zin Ko Winn on 1/12/2022.
//

#ifndef MINIBANK_USER_H
#define MINIBANK_USER_H

#include "iostream"

struct History {
    std::string transferFrom;
    std::string transferTo;
    std::string transactionTime;
    std::string transactionType;
    int amount;
    std::string notes;
};


struct User {
    int id;
    std::string userName;
    std::string password;
    std::string role;
    std::string phoneNumber;
    std::string email;
    int amount;
    std::string history;
};

#endif //MINIBANK_USER_H