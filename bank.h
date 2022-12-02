//
// Created by Zin Ko Winn on 1/12/2022.
//

#include "iostream"
#include "model/user.h"

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

        void showAllUser();

        bool isAdminUser(std::string userName);

        void deposit();

        int withDraw();

        void transfer();

        void history();

        bool isExist(std::string userName);

    private:
        std::string currentUserName;
        int balance;

        void update(User user);

        void showHistory(History history);

        void showData(User *user);
    };

}
