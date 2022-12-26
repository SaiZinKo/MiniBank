//
// Created by Zin Ko Winn on 12/25/2022.
//

#include <fstream>
#include <utility>
#include "data-store.h"
#include "iostream"
#include <iomanip>

using namespace DataBase;
using namespace std;

template<class T>
ZDataBase<T>::ZDataBase(){
    root = NULL;
}

template<class T>
void ZDataBase<T>::create(T data, Node<T> *rootNode) {

    if (!rootNode) {
        rootNode = (*this).root;
    }

    if (!(*this).root) {
        root = createNode(data);
        return;
    }

    if (data.id < (*this).root->data.id) {
        create((*this).root->left, data);
    } else {
        create((*this).root->right, data);
    }
}

template<class T>
void ZDataBase<T>::retrieveAllUser(Node<T> *root) {
    if (root == nullptr) return;
    retrieveAllUser(root->left);
    (*this).dataList.insert((*this).dataList.end(), root->data);
    retrieveAllUser(root->right);
}

template<class T>
list<T> ZDataBase<T>::findAll(Node<T> *node) {
    retrieveAllUser(node);
    list<T> tempDataList = (*this).dataList;
    (*this).dataList.clear();
    return tempDataList;
}

template<class T>
Node<T> *ZDataBase<T>::search(Node<T> *root, const T &data) {
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

template<class T>
void ZDataBase<T>::updateToFile(Node<T> *root, const T &user) {
    fstream tempFile;

    tempFile.open("tmp_user.txt", ios::app);

    if (!tempFile.is_open()) {
        cout << "File opening error" << endl;
        exit(1);
    }

    list<T> userList = ZDataBase::findAll(root);

    for (const T &tmpUser: userList) {
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

template<class T>
void ZDataBase<T>::update(Node<T> *root, const T &newData) {
    Node<T> *nodeToUpdate = search(root, newData);
    if (nodeToUpdate) {
        nodeToUpdate->data = newData;
        updateToFile(root, newData);
    }
}

template<class T>
void ZDataBase<T>::deleteUser(Node<T> *&root, const T &data) {
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
            Node<T> *temp = root;
            root = root->right;
            delete temp;
        } else if (!root->right) {
            Node<T> *temp = root;
            root = root->left;
            delete temp;
        } else {
            Node<T> *temp = root->right;
            while (temp->left) {
                temp = temp->left;
            }
            root->data = temp->data;
            deleteUser(root->right, temp->data);
        }
    }
}

template<class T>
void ZDataBase<T>::clear(Node<T> *node) {
    if (node != nullptr) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

template<class T>
bool ZDataBase<T>::isExists(Node<T> *&root, T data) {
    Node<T> *node = search(root, std::move(data));
    return node != nullptr;
}

template<class T>
T *ZDataBase<T>::findById(Node<T> *root, int id) {
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

template<class T>
void ZDataBase<T>::printTreeNode(Node<T> *rootNode, int indent) {
    if (!rootNode) {
        rootNode = (*this).root;
    }
    printTreeInOrder(rootNode->left);
    cout << setw(indent * 4) << rootNode->data << endl;
    printTreeInOrder(rootNode->right);
}
