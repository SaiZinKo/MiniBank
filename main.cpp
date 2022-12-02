//
// Created by Zin Ko Winn on 1/12/2022.
//

#include <fstream>
#include "bank.h"

using namespace Bank;
KBank kBank;

void initialData();

void mainMenu();

void adminMenu();

void userMenu();

int main() {
    initialData();
    mainMenu();
    return 0;
}

void initialData() {
    fstream file;
    file.open("user.txt", ios::app);

    if (!file.is_open()) {
        exit(1);
    }
    if (!kBank.isExist("admin")) {
        file << "admin" << ' ' << "admin" << ' ' << "ADMIN" << ' ' << "09" << ' ' << "admin@gmail.com" << ' ' << 0
             << ' '
             << "-" << '\n';
    }
    file.close();
}

void mainMenu() {
    int option = 0;
    do {
        cout << "===========================================" << endl;
        cout << "             WELCOME FROM KBank            " << endl;
        cout << "===========================================" << endl << endl;
        cout << "1. Account Registration" << endl;
        cout << "2. Account Login" << endl;
        cout << "3. Quit" << endl;
        cout << ">: ";
        cin >> option;

        switch (option) {
            case 1:
                kBank.registration();
                break;
            case 2: {
                User *user = kBank.login();
                kBank.setCurrentUserName(user->userName);
                kBank.setCurrentUserBalance(user->amount);
                kBank.isAdminUser(user->userName) ? adminMenu() : userMenu();
            }
                break;
            case 3:
                break;
        }
    } while (option != 3);
}

void adminMenu() {
    User user;
    int option = 0;
    do {
        cout << "===========================================" << endl;
        cout << "                 ADMIN MENU                " << endl;
        cout << "===========================================" << endl << endl;
        cout << "Name : " << kBank.getCurrentUserName() << endl << endl;
        cout << "1. View All Users" << endl;
        cout << "2. View All Transactions" << endl;
        cout << "3. Log Out" << endl;
        cout << ">: ";
        cin >> option;

        switch (option) {
            case 1:
                cout << "===========================================" << endl;
                cout << "               VIEW ALL USERS              " << endl;
                cout << "===========================================" << endl << endl;
                kBank.viewAllUsersInfo();
                break;
            case 2:
                cout << "===========================================" << endl;
                cout << "        VIEW ALL USERS TRANSACTIONS        " << endl;
                cout << "===========================================" << endl << endl;
                kBank.viewAllUsersTransactions();
                break;
        }
    } while (option != 3);
}

void userMenu() {
    int option = 0;
    do {
        cout << "===========================================" << endl;
        cout << "                  USER MENU                " << endl;
        cout << "===========================================" << endl << endl;
        cout << "Name : " << kBank.getCurrentUserName() << ", " << "Balance : "
             << to_string(kBank.getCurrentUserBalance()) << "Kyats" << endl << endl;
        cout << "1. View Profile" << endl;
        cout << "2. Deposit" << endl;
        cout << "3. Cash WithDraw" << endl;
        cout << "4. Transfer" << endl;
        cout << "5. History" << endl;
        cout << "6. Change Password" << endl;
        cout << "7. Log Out" << endl;
        cout << ">: ";
        cin >> option;

        switch (option) {
            case 1: {
                cout << "===========================================" << endl;
                cout << "                VIEW PROFILE               " << endl;
                cout << "===========================================" << endl << endl;
                kBank.viewProfile();
            }
                break;
            case 2: {
                cout << "===========================================" << endl;
                cout << "                   DEPOSIT                 " << endl;
                cout << "===========================================" << endl << endl;
                kBank.deposit();
            }
                break;
            case 3: {
                cout << "===========================================" << endl;
                cout << "               CASH WITHDRAW               " << endl;
                cout << "===========================================" << endl << endl;
                int status = kBank.withDraw();
                if (status == 0) {
                    cout << "Your money is nor avali" << endl << endl;
                }
            }
                break;
            case 4: {
                cout << "===========================================" << endl;
                cout << "                 TRANSFER                  " << endl;
                cout << "===========================================" << endl << endl;
                kBank.transfer();
            }
                break;
            case 5: {
                cout << "===========================================" << endl;
                cout << "                  HISTORY                  " << endl;
                cout << "===========================================" << endl << endl;
                kBank.history();
            }
                break;
            case 6: {
                cout << "===========================================" << endl;
                cout << "              CHANGE PASSWORD              " << endl;
                cout << "===========================================" << endl << endl;
                kBank.changePassword();
            }
                break;
        }
    } while (option != 7);
}