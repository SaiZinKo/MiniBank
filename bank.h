//
// Created by Zin Ko Winn on 1/12/2022.
//

#ifndef MINIBANK_BANK_H
#define MINIBANK_BANK_H

#include "iostream"
#include "model/user.h"
#include "user-service.h"

using namespace std;

namespace Bank {

    class KBank {
    public:
        void registration();

        User *login() const;

        static User findByUserName(const std::string &userName);

        void setCurrentUserName(string userName);

        string getCurrentUserName();

        void setCurrentUserBalance(int amount);

        int getCurrentUserBalance() const;

        void setCurrentUserId(int id);

        void viewAllUsersInfo();

        void viewAllUsersTransactions();

        static bool isAdminUser(User *user);

        void deposit();

        int withDraw();

        void transfer();

        void history();

        void changePassword() const;

        void viewProfile() const;

        bool isExist(const std::string &userName);

        static void showData(User *user);

        Node *root = nullptr;

    private:
        std::string currentUserName;
        int userId;
        int balance;

        void showHistory(const User &user);

        static void printHistory(const History &history, const string &userName);
    };
}

#endif //MINIBANK_BANK_H
