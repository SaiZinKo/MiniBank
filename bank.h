//
// Created by Zin Ko Winn on 1/12/2022.
//

#include "iostream"
#include "model/user.h"
#include "data-store.h"

using namespace std;

namespace Bank {

    class KBank {
    public:
        void registration();

        User *login();

        User findByUserName(const std::string &userName);

        void setCurrentUserName(string userName);

        string getCurrentUserName();

        void setCurrentUserBalance(int amount);

        int getCurrentUserBalance();

        void setCurrentUserId(int id);

        int getCurrentUserId();

        void viewAllUsersInfo();

        void viewAllUsersTransactions();

        bool isAdminUser(User *user);

        void deposit();

        int withDraw();

        void transfer();

        void history();

        void changePassword();

        void viewProfile();

        bool isExist(std::string userName);

        static void showData(User *user);

        Node *root = nullptr;
    private:
        std::string currentUserName;
        int userId;
        int balance;

        void showHistory(User user);

        void printHistory(History history, string userName);
    };

}
