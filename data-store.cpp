//
// Created by Zin Ko Winn on 12/25/2022.
//

#include <fstream>
#include "data-store.h"
#include "iostream"
#include "bank.h"

using namespace BankData;
using namespace Bank;
list <User> userList;

Node *KBankData::createNode(User data) {
    Node *newNode = new Node;
    newNode->data = data;
    newNode->left = nullptr;
    newNode->right = nullptr;
    return newNode;
}

void KBankData::insert(Node *&root, User data) {
    if (!root) {
        root = createNode(data);
        return;
    }
    if (data.id < root->data.id) {
        insert(root->left, data);
    } else {
        insert(root->right, data);
    }
}

Node *KBankData::search(Node *root, User data) {
    if (!root) {
        return nullptr;
    } else if (root->data.userName == data.userName) {
        return root;
    } else if (data.id < root->data.id) {
        return search(root->left, data);
    } else {
        return search(root->right, data);
    }
}

void KBankData::updateToFile(Node *root, User user) {
    fstream tempFile;

    tempFile.open("tmp_user.txt", ios::app);

    if (!tempFile.is_open()) {
        cout << "File opening error" << endl;
        exit(1);
    }

    list<User> userList = KBankData::findAll(root);

    for (User tmpUser: userList) {
        if (tmpUser.userName == user.userName) {
            tempFile << user.id << ' ' << user.userName << ' ' << user.password << ' ' << user.role << ' '
                     << user.phoneNumber
                     << ' '
                     << user.email << ' ' << user.amount << ' ' << user.history << ' ' << '\n';
        } else {
            tempFile << tmpUser.id << ' ' << tmpUser.userName << ' ' << tmpUser.password << ' ' << tmpUser.role
                     << ' '
                     << tmpUser.phoneNumber
                     << ' ' << tmpUser.email << ' ' << tmpUser.amount << ' ' << tmpUser.history
                     << ' '
                     << '\n';
        }
    }

    tempFile.close();
    remove("user.txt");
    rename("tmp_user.txt", "user.txt");
    cout << "User info updated successfully." << endl << endl;
}

void KBankData::update(Node *root, User newData) {
    Node *nodeToUpdate = search(root, newData);
    if (nodeToUpdate) {
        nodeToUpdate->data = newData;
        updateToFile(root, newData);
    }
}

void KBankData::deleteUser(Node *&root, User data) {
    if (!root) {
        return;
    } else if (data.id < root->data.id) {
        deleteUser(root->left, data);
    } else if (data.id > root->data.id) {
        deleteUser(root->right, data);
    } else {
        if (!root->left && !root->right) {
            delete root;
            root = nullptr;
        } else if (!root->left) {
            Node *temp = root;
            root = root->right;
            delete temp;
        } else if (!root->right) {
            Node *temp = root;
            root = root->left;
            delete temp;
        } else {
            Node *temp = root->right;
            while (temp->left) {
                temp = temp->left;
            }
            root->data = temp->data;
            deleteUser(root->right, temp->data);
        }
    }
}

void KBankData::clear(Node *node) {
    if (node != nullptr) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

bool KBankData::isExists(Node *&root, User user) {
    Node *node = search(root, user);
    return node != nullptr;
}

void KBankData::retrieveAllUser(Node *root) {
    if (root == nullptr) return;
    retrieveAllUser(root->left);
    userList.insert(userList.end(), root->data);
    retrieveAllUser(root->right);
}

list<User> KBankData::findAll(Node *node) {
    retrieveAllUser(node);
    list<User> tempUserList = userList;
    userList.clear();
    return tempUserList;
}

User *KBankData::findById(Node *root, int id) {
    if (!root) {
        return nullptr;
    } else if (root->data.id == id) {
        return &root->data;
    } else if (id < root->data.id) {
        return findById(root->left, id);
    } else {
        return findById(root->right, id);
    }
}

void KBankData::printTreeInOrder(Node *node) {
    if (node == nullptr) return;

    printTreeInOrder(node->left);
    KBank::showData(&node->data);
    printTreeInOrder(node->right);
}
