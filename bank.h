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

        User findByUserName(std::string userName);

        void setCurrentUserName(string userName);

        string getCurrentUserName();

        void showAllUser();

        bool isAdminUser(std::string userName);

        void deposit();

        void withDraw();

    private:
        std::string currentUserName;

        bool isExist(std::string userName);

        void update(User user);

        void showData(User *user);
    };

}
