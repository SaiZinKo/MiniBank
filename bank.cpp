//
// Created by Zin Ko Winn on 1/12/2022.
//

#include <fstream>
#include <utility>
#include "ctime"
#include "bank.h"
#include "common-utils.h"

using namespace Bank;
using namespace BankData;

History depositAndWithDrawHistory(int amount, string transactionTime, string transactionType) {
    History history;
    history.transferFrom = "-";
    history.transferTo = "-";
    history.transactionTime = std::move(transactionTime);
    history.transactionType = std::move(transactionType);
    history.amount = amount;
    history.notes = "-";
    return history;
}

string historyToString(const History &history) {
    return history.transferFrom + ',' + history.transferTo + ',' + history.transactionTime + ',' +
           history.transactionType + ',' + to_string(history.amount) + ',' + history.notes + '|';
}

string timeStampToDateTime(const char *timeStamp) {
    time_t t = atoi(timeStamp);
    return asctime(localtime(&t));
}

string getCurrentTimeStamp() {
    return to_string(time(nullptr));
}

History
transferHistory(string transferFrom, string transferTo, string transactionTime, string transactionType, int amount,
                string notes) {
    History history;
    history.transferFrom = std::move(transferFrom);
    history.transferTo = std::move(transferTo);
    history.transactionTime = std::move(transactionTime);
    history.transactionType = std::move(transactionType);
    history.amount = amount;
    history.notes = std::move(notes);
    return history;
}

void saveFile(const User &user) {
    fstream file;
    file.open("user.txt", ios::app);

    if (!file.is_open()) {
        cout << "File opening error" << endl;
        exit(1);
    }

    file << user.id << ' ' << user.userName << ' ' << user.password << ' ' << "USER" << ' ' << user.phoneNumber << ' '
         << user.email << ' ' << user.amount << ' ' << user.history << ' ' << '\n';
    file.close();
}

void KBank::registration() {
    User user;

    cout << "===========================================" << endl;
    cout << "             Account Registration          " << endl;
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
    string transactionHistory = historyToString(
            depositAndWithDrawHistory(user.amount, getCurrentTimeStamp(), "DEPOSIT"));
    user.history = transactionHistory;
    user.id = generateId();
    user.role = "USER";

    if (KBankData::isExists(root, user)) {
        cout << "User name " << user.userName << " is already exists." << endl << endl;
        return;
    }
    KBankData::insert(root, user);
    saveFile(user);
}

User *KBank::login() const {
    User *userPtr = nullptr;
    string userName;
    string password;
    bool isValidUser = true;

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

        list<User> userList = KBankData::findAll(root);

        for (User user: userList) {
            isValidUser = user.userName == userName && user.password == password;

            if (isValidUser) {
                cout << "Login Success" << endl << endl;
                userPtr = &user;
                return userPtr;
            }
        }
    } while (!isValidUser);
    return userPtr;
}

void KBank::viewProfile() const {
    User *user = KBankData::findById(root, userId);
    showData(user);
}

void KBank::deposit() {
    User user = *KBankData::findById(root, userId);
    int amount = 0;
    cout << "Enter amount : ";
    cin >> amount;
    cout << endl;
    user.amount += amount;
    if (user.history == "-") {
        user.history = historyToString(depositAndWithDrawHistory(amount, getCurrentTimeStamp(), "DEPOSIT"));
    } else {
        user.history += historyToString(depositAndWithDrawHistory(amount, getCurrentTimeStamp(), "DEPOSIT"));
    }
    KBankData::update(root, user);
    setCurrentUserBalance(user.amount);
}

int KBank::withDraw() {
    User user = *KBankData::findById(root, userId);
    int amount = 0;
    cout << "Enter amount : ";
    cin >> amount;
    cout << endl;
    if (user.amount > amount) {
        user.amount -= amount;
        if (user.history == "-") {
            user.history = historyToString(depositAndWithDrawHistory(amount, getCurrentTimeStamp(), "WITHDRAW"));
        } else {
            user.history += historyToString(
                    depositAndWithDrawHistory(amount, getCurrentTimeStamp(), "WITHDRAW"));
        }
        KBankData::update(root, user);
        setCurrentUserBalance(user.amount);
        return 1;
    }
    return 0;
}

void KBank::transfer() {
    string transferTo;
    string notes;
    int amount;
    User currentUser = findByUserName(currentUserName);
    cout << "Enter transfer to user name : ";
    cin >> transferTo;
    if (isExist(transferTo)) {
        User transferToUser = findByUserName(transferTo);
        cout << "Enter amount to transfer : ";
        cin >> amount;
        if (currentUser.amount > amount) {
            transferToUser.amount += amount;
            currentUser.amount -= amount;
            cout << "Enter notes : ";
            cin >> notes;
            if (currentUser.history == "-") {
                currentUser.history = historyToString(
                        transferHistory(currentUserName, transferTo, getCurrentTimeStamp(), "TRANSFER", amount, notes));
            } else {
                currentUser.history += historyToString(
                        transferHistory(currentUserName, transferTo, getCurrentTimeStamp(), "TRANSFER", amount, notes));
            }

            if (transferToUser.history == "-") {
                transferToUser.history = historyToString(
                        transferHistory(currentUserName, transferTo, getCurrentTimeStamp(), "TRANSFER", amount, notes));
            } else {
                transferToUser.history += historyToString(
                        transferHistory(currentUserName, transferTo, getCurrentTimeStamp(), "TRANSFER", amount, notes));
            }
            KBankData::update(root, transferToUser);
            KBankData::update(root, currentUser);
            setCurrentUserBalance(currentUser.amount);
        } else {
            cout << "Invalid" << endl << endl;
        }
    } else {
        cout << transferTo << " is not exist" << endl << endl;
    }


    cout << "Transferred money successfully" << endl << endl;
}

void KBank::history() {
    User user = *KBankData::findById(root, userId);
    showHistory(user);
}

void KBank::changePassword() const {
    User user = *KBankData::findById(root, userId);
    string oldPassword;
    string newPassword;
    string confirmPassword;
    cout << "Old Password : ";
    cin >> oldPassword;
    cout << "New Password : ";
    cin >> newPassword;
    cout << "Confirm Password : ";
    cin >> confirmPassword;
    cout << endl;

    if (oldPassword == user.password) {
        if (newPassword == confirmPassword) {
            user.password = newPassword;
            KBankData::update(root, user);
        } else {
            cout << "New password and confirm password must be same" << endl << endl;
        }
    } else {
        cout << "Old password is incorrect" << endl << endl;
        return;
    }

    cout << "Password changed successfully. Your new password is " << newPassword << endl << endl;
}

bool KBank::isAdminUser(User *user) {
    return user->role == "ADMIN";
}

bool KBank::isExist(const std::string &userName) {
    User user = findByUserName(userName);
    return !user.userName.empty();
}

void KBank::setCurrentUserName(string userName) {
    currentUserName = std::move(userName);
}

string KBank::getCurrentUserName() {
    return currentUserName;
}

void KBank::setCurrentUserBalance(int amount) {
    balance = amount;
}

int KBank::getCurrentUserBalance() const {
    return balance;
}

void KBank::setCurrentUserId(int id) {
    userId = id;
}

User KBank::findByUserName(const std::string &userName) {
    User user;
    fstream file;
    file.open("user.txt", ios::in);

    if (!file.is_open()) {
        cout << "File opening error" << endl;
        exit(1);
    }

    while (!file.eof()) {
        file >> user.id >> user.userName >> user.password >> user.role >> user.phoneNumber >> user.email >> user.amount
             >> user.history;
        if (user.userName == userName) {
            file.close();
            return user;
        }
    }
    file.close();
    user.userName = "";
    return user;
}

void KBank::viewAllUsersInfo() {
    list<User> userList = KBankData::findAll(root);

    if (userList.empty()) {
        cout << "There is no user data." << endl << endl;
        return;
    }
    for (User &user: userList) {
        showData(&user);
    }
}

void KBank::viewAllUsersTransactions() {
    User *previousUser;
    list<User> userList = KBankData::findAll(root);
    if (userList.size() == 1) {
        cout << "There is no user data." << endl << endl;
        return;
    }
    for (User &user: userList) {
        if (user.role != "ADMIN") {
            if (!previousUser || previousUser->userName != user.userName) {
                cout << "* The transaction history of " << user.userName << endl << endl;
            }
            showHistory(user);
            previousUser = &user;
        }
    }
}

void KBank::showData(User *user) {
    cout << "User Name : " << user->userName << endl;
    cout << "Password : " << user->password << endl;
    cout << "Role : " << user->role << endl;
    if (user->role != "ADMIN") {
        cout << "Phone Number : " << user->phoneNumber << endl;
        cout << "Email : " << user->email << endl;
        cout << "Amount : " << user->amount << endl << endl;
    }else{
        cout << endl;
    }
}

void KBank::showHistory(const User &user) {
    History history;
    int index = 0;
    string value;
    for (auto &ch: user.history) {
        if (ch == '|') {
            history.notes = value;
            printHistory(history, user.userName);
            value = "";
            index = 0;
        } else if (ch == ',') {
            index++;
            if (index == 1) history.transferFrom = value;
            if (index == 2) history.transferTo = value;
            if (index == 3) history.transactionTime = value;
            if (index == 4) history.transactionType = value;
            if (index == 5) history.amount = stoi(value);
            value = "";
        } else {
            string st(1, ch);
            value += st;
        }
    }
}

void KBank::printHistory(const History &history, const string &userName) {
    string sign = history.transactionType == "WITHDRAW" || history.transferFrom == userName ? "-" : "+";

    if (history.transactionType == "DEPOSIT" || history.transactionType == "WITHDRAW") {
        cout << "Transaction Type : " << history.transactionType << endl;
        cout << "Transaction Time : " << timeStampToDateTime(history.transactionTime.c_str());
        cout << "Amount : " << sign << history.amount << "Kyats" << endl << endl;
    }

    if (history.transactionType == "TRANSFER") {
        cout << "Transfer From : " << history.transferFrom << endl;;
        cout << "Transfer To : " << history.transferTo << endl;;
        cout << "Transaction Type : " << history.transactionType << endl;
        cout << "Transaction Time : " << timeStampToDateTime(history.transactionTime.c_str());
        cout << "Amount : " << sign << history.amount << "Kyats" << endl;
        cout << "Notes : " << history.notes << endl << endl;
    }
}