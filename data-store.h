//
// Created by Zin Ko Winn on 12/25/2022.
//

#ifndef MINIBANK_DATA_STORE_H
#define MINIBANK_DATA_STORE_H

#include "list"

namespace DataBase {
    template<class T>
    struct Node {
        T data;
        Node *left, *right{};
    };

    template<class T>
    class ZDataBase {
    private:
        Node<T> *root;
        std::list<T> dataList;

        Node<T> *createNode(T data) {
            Node<T> *newNode = new Node<T>;
            newNode->data = std::move(data);
            newNode->left = nullptr;
            newNode->right = nullptr;
            return newNode;
        }

        void retrieveAllUser(Node<T> *rootNode);

        void updateToFile(Node<T> *rootNode, const T &user);

    public:
        ZDataBase();

        void create(T data, Node<T> *rootNode = nullptr);

        std::list<T> findAll(Node<T> *rootNode);

        void update(Node<T> *rootNode, const T &newData);

        Node<T> *search(Node<T> *rootNode, const T &data);

        void deleteUser(Node<T> *&rootNode, const T &data);

        void clear(Node<T> *rootNode);

        bool isExists(Node<T> *&rootNode, T user);

        T *findById(Node<T> *rootNode, int id);

        void printTreeNode(Node<T> *rootNode, int indent = 1);

    };
}
#endif //MINIBANK_DATA_STORE_H
