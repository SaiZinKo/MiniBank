//
// Created by Zin Ko Winn on 1/12/2022.
//

#include <fstream>
#include "bank.h"
#include "ctime"

using namespace Bank;

void KBank::registration() {
    char DELIMITER = '|';
    User user;

    fstream file;
    file.open("user.txt", ios::app);

    if (!file.is_open()) {
        cout << "File opening error" << endl;
        exit(1);
    }

    cout << "===========================================" << endl;
    cout << "             Account Registration            " << endl;
    cout << "===========================================" << endl << endl;
    cout << "Enter Use Name : ";
    cin >> user.userName;
    cout << "Enter Password : ";
    cin >> user.password;
    cout << "Enter Phone Number : ";
    cin >> user.phoneNumber;
    cout << "Enter Email : ";
    cin >> user.email;
    cout << "Enter Amount : ";
    cin >> user.amount;
    cout << endl;
    file << user.userName << ' ' << user.password << ' ' << "USER" << ' ' << user.phoneNumber << ' ' << user.email
         << ' ' << user.amount << ' ' << DELIMITER << ' ' << "-" << ' ' << '\n';
    file.close();
    cout << "Account Registration Success" << endl << endl;
}

User *KBank::login() {
    User user;
    User *userPtr = nullptr;
    string userName;
    string password;
    bool isValidUser = true;
    string DELIMITER;

    fstream file;
    file.open("user.txt", ios::in);

    if (!file.is_open()) {
        cout << "File opening error" << endl;
        exit(1);
    }
    do {
        if (!isValidUser) {
            cout << endl << "Please try again! Invalid User Name or Password" << endl << endl;
        }
        cout << "===========================================" << endl;
        cout << "                Account Login              " << endl;
        cout << "===========================================" << endl << endl;
        cout << "Enter Use Name : ";
        cin >> userName;
        cout << "Enter Password : ";
        cin >> password;

        while (!file.eof()) {
            file >> user.userName >> user.password >> user.role >> user.phoneNumber >> user.email >> user.amount
                 >> DELIMITER >> user.history;

            isValidUser = user.userName == userName && user.password == password;

            if (isValidUser) {
                cout << "Login Success" << endl << endl;
                userPtr = &user;
                file.close();
                return userPtr;
            }
        }
    } while (!isValidUser);
    file.close();
    return userPtr;
}

bool KBank::isExist(std::string userName) {
    User user = findByUserName(userName);
    return !user.userName.empty();
}

string KBank::getCurrentUserName() {
    return currentUserName;
}

void KBank::setCurrentUserName(string userName) {
    currentUserName = userName;
}

User KBank::findByUserName(std::string userName) {
    User user;
    string DELIMITER;
    fstream file;
    file.open("user.txt", ios::in);

    if (!file.is_open()) {
        cout << "File opening error" << endl;
        exit(1);
    }

    while (!file.eof()) {
        file >> user.userName >> user.password >> user.role >> user.phoneNumber >> user.email >> user.amount
             >> DELIMITER >> user.history;
        if (user.userName == userName) {
            file.close();
            return user;
        }
    }
    file.close();

    return user;
}

void KBank::showAllUser() {
    User user;
    User *userPtr;
    fstream file;
    string DELIMITER;
    file.open("user.txt", ios::in);

    if (!file.is_open()) {
        cout << "File opening error" << endl;
    }
    while (!file.eof()) {
        file >> user.userName >> user.password >> user.role >> user.phoneNumber >> user.email >> user.amount
             >> DELIMITER >> user.history;

        userPtr = user.userName.empty() ? nullptr : &user;
        if (userPtr != nullptr) {
            if (!userPtr->userName.empty()) {
                showData(userPtr);
                user.userName = "";
            } else {
                cout << "There is no user data." << endl << endl;
            }
        }
    }
    file.close();
}

bool KBank::isAdminUser(std::string userName) {
    User user = findByUserName(userName);
    return user.role == "ADMIN";
}

void KBank::showData(User *user) {
    cout << "User Name : " << user->userName << endl;
    cout << "Password : " << user->password << endl;
    cout << "Role : " << user->role << endl;
    cout << "Phone Number : " << user->phoneNumber << endl;
    cout << "Email : " << user->email << endl;
    cout << "Amount : " << user->amount << endl << endl;
}

History depositHistory(int amount, string transactionTime) {
    History history;
    history.transferFrom = "-";
    history.transferTo = "-";
    history.transactionTime = transactionTime;
    history.transactionType = "deposit";
    history.amount = amount;
    history.notes = "-";
    return history;
}

History stringToHistory(std::string historyString) {

}

string historyToString(History history) {
    return history.transferFrom + ' ' + history.transferTo + ' ' + history.transactionTime + ' ' +
           history.transactionType + ' ' + to_string(history.amount) + ' ' + history.notes;
}

void KBank::deposit() {
    User user = findByUserName(currentUserName);
    int amount = 0;
    cout << "Enter amount : ";
    cin >> amount;
    cout << endl;
    user.amount += amount;
    user.history = historyToString(depositHistory(amount, to_string(time(NULL))));
    update(user);
}

string timeStampToDateTime(const char *timeStamp) {
    time_t t = atoi(timeStamp);
    return asctime(localtime(&t));
}

void KBank::withDraw() {
    User user = findByUserName(currentUserName);
    int amount = 0;
    cout << "Enter amount : ";
    cin >> amount;
    cout << endl;
    user.amount -= amount;
//    user.history = historyToString(depositHistory(amount, to_string(time(NULL))));
    update(user);
}

void KBank::update(User user) {
    fstream file;
    fstream tempFile;
    User tmpUser;
    User *tmpUserPtr;
    string DELIMITER;

    file.open("user.txt", ios::in);
    tempFile.open("tmp_user.txt", ios::app);

    if (!file.is_open() || !tempFile.is_open()) {
        cout << "File opening error" << endl;
        exit(1);
    }

    while (!file.eof()) {
        file >> tmpUser.userName >> tmpUser.password >> tmpUser.role >> tmpUser.phoneNumber >> tmpUser.email
             >> tmpUser.amount >> DELIMITER >> tmpUser.history;

        tmpUserPtr = tmpUser.userName.empty() ? nullptr : &tmpUser;
        if (tmpUserPtr != nullptr) {
            if (tmpUser.userName == user.userName) {
                tempFile << user.userName << ' ' << user.password << ' ' << user.role << ' ' << user.phoneNumber
                         << ' '
                         << user.email << ' ' << user.amount << ' ' << DELIMITER << ' ' << user.history << ' ' << '\n';
            } else {
                tempFile << tmpUser.userName << ' ' << tmpUser.password << ' ' << tmpUser.role << ' '
                         << tmpUser.phoneNumber
                         << ' ' << tmpUser.email << ' ' << tmpUser.amount << ' ' << DELIMITER << ' ' << tmpUser.history
                         << ' '
                         << '\n';
            }
            tmpUser.userName = "";
        }
    }

    file.close();
    tempFile.close();
    remove("user.txt");
    rename("tmp_user.txt", "user.txt");
}

void showHistory(History history) {
    if (history.transactionType == "deposit" || history.transactionType == "WITHDRAW") {
        string sign = history.transactionType == "WITHDRAW" ? "-" : "+";
        cout << "Transaction Type : " << history.transactionType << endl;;
        cout << "Transaction Time : " << timeStampToDateTime(history.transactionTime.c_str());
        cout << "Amount : " << sign << history.amount << endl << endl;
    }

    if (history.transactionType == "TRANSFER") {
        cout << "Transfer From : " << history.transferFrom << endl;;
        cout << "Transfer To : " << history.transferTo << endl;;
        cout << "Transaction Type : " << history.transactionType << endl;;
        cout << "Transaction Time : " << timeStampToDateTime(history.transactionTime.c_str());
        cout << "Amount : " << history.amount << endl << endl;
    }
}

void KBank::history() {
    History history;
    User user = findByUserName(currentUserName);
    int index = 0;
    string value;
    for (auto &ch: user.history) {
        if (ch == '|') {
            showHistory(history);
            index = 0;
        } else if (ch == ',') {
            index++;
            if (index == 1) history.transferFrom = value;
            if (index == 2) history.transferTo = value;
            if (index == 3) history.transactionTime = value;
            if (index == 4) history.transactionType = value;
            if (index == 5) history.amount = stoi(value);;
            if (index == 6) history.notes = value;
            value = "";
        } else {
            string st(1, ch);
            value += st;
        }
    }
}

//zinkowin zinkowin USER 09797256920 zinko.developer@gmail.com 3500 | -,-,1669917698,deposit,500,-|-,-,1669917698,WITHDRAW,500,-|