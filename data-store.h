//
// Created by Zin Ko Winn on 12/25/2022.
//

#ifndef MINIBANK_DATA_STORE_H
#define MINIBANK_DATA_STORE_H

#include <iostream>
#include "list"
#include "model/user.h"

using namespace std;

struct Node {
    User data;
    Node *left{};
    Node *right{};
};

namespace BankData {
    class KBankData {

    public:
        static Node *createNode(User data);

        static void insert(Node *&root, const User& data);

        static Node *search(Node *root, const User& data);

        static void update(Node *root, const User& newData);

        static void deleteUser(Node *&root, const User& data);

        static void clear(Node *root);

        static bool isExists(Node *&root, User user);

        static list <User> findAll(Node *node);

        static User *findById(Node *root, int id);

        static void printTreeInOrder(Node *node);

    private:
        static void retrieveAllUser(Node *root);

        static void updateToFile(Node *root, const User& user);
    };
}

#endif //MINIBANK_DATA_STORE_H
