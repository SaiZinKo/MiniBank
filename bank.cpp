//
// Created by Zin Ko Winn on 1/12/2022.
//

#include <fstream>
#include "bank.h"
#include "ctime"

using namespace Bank;

History depositAndWithDrawHistory(int amount, string transactionTime, string transactionType) {
    History history;
    history.transferFrom = "-";
    history.transferTo = "-";
    history.transactionTime = transactionTime;
    history.transactionType = transactionType;
    history.amount = amount;
    history.notes = "-";
    return history;
}

string historyToString(History history) {
    return history.transferFrom + ',' + history.transferTo + ',' + history.transactionTime + ',' +
           history.transactionType + ',' + to_string(history.amount) + ',' + history.notes + '|';
}

string timeStampToDateTime(const char *timeStamp) {
    time_t t = atoi(timeStamp);
    return asctime(localtime(&t));
}

string getCurrentTimeStamp() {
    return to_string(time(NULL));
}

History
transferHistory(string transferFrom, string transferTo, string transactionTime, string transactionType, int amount,
                string notes) {
    History history;
    history.transferFrom = transferFrom;
    history.transferTo = transferTo;
    history.transactionTime = transactionTime;
    history.transactionType = transactionType;
    history.amount = amount;
    history.notes = notes;
    return history;
}

void KBank::registration() {
    User user;

    fstream file;
    file.open("user.txt", ios::app);

    if (!file.is_open()) {
        cout << "File opening error" << endl;
        exit(1);
    }

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
    if (!isExist(user.userName)) {
        file << user.userName << ' ' << user.password << ' ' << "USER" << ' ' << user.phoneNumber << ' '
             << user.email << ' ' << user.amount << ' ' << transactionHistory << ' ' << '\n';
    } else {
        cout << "User name " << user.userName << " is already exists." << endl << endl;
    }
    file.close();
}

User *KBank::login() {
    User user;
    User *userPtr = nullptr;
    string userName;
    string password;
    bool isValidUser = true;

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
                 >> user.history;

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

void KBank::viewProfile() {
    User user = findByUserName(currentUserName);
    showData(&user);
}

void KBank::deposit() {
    User user = findByUserName(currentUserName);
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
    update(user);
    setCurrentUserBalance(user.amount);
}

int KBank::withDraw() {
    User user = findByUserName(currentUserName);
    int amount = 0;
    cout << "Enter amount : ";
    cin >> amount;
    cout << endl;
    if (user.amount > amount) {
        user.amount -= amount;
        if (user.history == "-") {
            user.history = historyToString(depositAndWithDrawHistory(amount, getCurrentTimeStamp(), "WITHDRAW"));
        } else {
            user.history += historyToString(depositAndWithDrawHistory(amount, getCurrentTimeStamp(), "WITHDRAW"));
        }
        update(user);
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
            update(transferToUser);
            update(currentUser);
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
    User user = findByUserName(currentUserName);
    showHistory(user);
}

void KBank::changePassword() {
    User user = findByUserName(currentUserName);
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
            update(user);
        } else {
            cout << "New password and confirm password must be same" << endl << endl;
        }
    } else {
        cout << "Old password is incorrect" << endl << endl;
    }

    cout << "Password changed successfully. Your new password is " << newPassword << endl << endl;
}

bool KBank::isAdminUser(std::string userName) {
    User user = findByUserName(userName);
    return user.role == "ADMIN";
}

bool KBank::isExist(std::string userName) {
    User user = findByUserName(userName);
    return !user.userName.empty();
}

void KBank::setCurrentUserName(string userName) {
    currentUserName = userName;
}

string KBank::getCurrentUserName() {
    return currentUserName;
}

void KBank::setCurrentUserBalance(int amount) {
    balance = amount;
}

int KBank::getCurrentUserBalance() {
    return balance;
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
        file >> user.userName >> user.password >> user.role >> user.phoneNumber >> user.email >> user.amount
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
    User user;
    User *userPtr;
    fstream file;
    file.open("user.txt", ios::in);

    if (!file.is_open()) {
        cout << "File opening error" << endl;
    }
    while (!file.eof()) {
        file >> user.userName >> user.password >> user.role >> user.phoneNumber >> user.email >> user.amount
             >> user.history;

        userPtr = user.userName == "" ? nullptr : &user;
        if (userPtr != nullptr) {
            if (!userPtr->userName.empty()) {
                if (userPtr->role != "ADMIN") {
                    showData(userPtr);
                }
                user.userName = "";
            } else {
                cout << "There is no user data." << endl << endl;
            }
        }
    }
    file.close();
}

void KBank::viewAllUsersTransactions() {
    User user;
    User *userPtr;
    fstream file;
    History history;
    file.open("user.txt", ios::in);

    if (!file.is_open()) {
        cout << "File opening error" << endl;
    }
    while (!file.eof()) {
        file >> user.userName >> user.password >> user.role >> user.phoneNumber >> user.email >> user.amount
             >> user.history;

        userPtr = user.userName == "" ? nullptr : &user;
        if (userPtr != nullptr) {
            if (!userPtr->userName.empty()) {
                if (userPtr->role != "ADMIN") {
                    showHistory(*userPtr);
                }
                user.userName = "";
            } else {
                cout << "There is no user data." << endl << endl;
            }
        }
    }
    file.close();
}

void KBank::update(User user) {
    fstream file;
    fstream tempFile;
    User tmpUser;
    User *tmpUserPtr;

    file.open("user.txt", ios::in);
    tempFile.open("tmp_user.txt", ios::app);

    if (!file.is_open() || !tempFile.is_open()) {
        cout << "File opening error" << endl;
        exit(1);
    }

    while (!file.eof()) {
        file >> tmpUser.userName >> tmpUser.password >> tmpUser.role >> tmpUser.phoneNumber >> tmpUser.email
             >> tmpUser.amount >> tmpUser.history;

        tmpUserPtr = tmpUser.userName.empty() ? nullptr : &tmpUser;
        if (tmpUserPtr != nullptr) {
            if (tmpUser.userName == user.userName) {
                tempFile << user.userName << ' ' << user.password << ' ' << user.role << ' ' << user.phoneNumber
                         << ' '
                         << user.email << ' ' << user.amount << ' ' << user.history << ' ' << '\n';
            } else {
                tempFile << tmpUser.userName << ' ' << tmpUser.password << ' ' << tmpUser.role << ' '
                         << tmpUser.phoneNumber
                         << ' ' << tmpUser.email << ' ' << tmpUser.amount << ' ' << tmpUser.history
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
    cout << "User info updated successfully." << endl << endl;
}

void KBank::showData(User *user) {
    cout << "User Name : " << user->userName << endl;
    cout << "Password : " << user->password << endl;
    cout << "Role : " << user->role << endl;
    cout << "Phone Number : " << user->phoneNumber << endl;
    cout << "Email : " << user->email << endl;
    cout << "Amount : " << user->amount << endl << endl;
}

void KBank::showHistory(User user) {
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
            if (index == 5) history.amount = stoi(value);;
            value = "";
        } else {
            string st(1, ch);
            value += st;
        }
    }
}

void KBank::printHistory(History history, string userName) {
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