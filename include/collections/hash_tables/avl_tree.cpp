//
// Created by Mikolaj Jalocha on 01/06/2025.
//

#include "avl_tree.h"

#include <algorithm>
#include <iostream>

AVLTree::AVLTree() : root(nullptr), size(0){ }

void AVLTree::clear(NodeAVL* node) {
    if (!node) return;
    clear(node->left);
    clear(node->right);
    delete node;
}

AVLTree::~AVLTree() {
    clear(root);
    size =0;
}

int AVLTree::height(NodeAVL* node)
{
    return node ? node->height : 0;
}

int AVLTree::balance_factor(NodeAVL* node)
{
    return node ? height(node->left) - height(node->right) : 0;
}

NodeAVL* AVLTree::insert(NodeAVL* node, int key, int value){

    if (node == nullptr) {
        size++;
        return new NodeAVL(key, value);
    }

    if (key < node->key) {
        node->left = insert(node->left, key, value);
    }
    else if (key > node->key) {
        node -> right = insert(node->right, key, value);
    } else {
        node->value=value;
        return node;
    }
    node->height = std::max(height(node->left), height(node->right)) +1;
    if (balance_factor(node) > 1) {
        if (node->left && key < node->left->key)
            return right_rotate(node);
        if (node->left && key > node->left->key) {
            node->left = left_rotate(node->left);
            return right_rotate(node);
        }
    }
    if (balance_factor(node) < -1) {
        if (node->right && key > node->right->key)
            return left_rotate(node);
        if (node->right && key < node->right->key) {
            node->right = right_rotate(node->right);
            return left_rotate(node);
        }
    }

    return node;
}

NodeAVL* AVLTree::left_rotate(NodeAVL* node) {
    NodeAVL *r_child(node->right);
    NodeAVL* rl_grandchild(r_child->left);

    r_child->left = node;
    node->right = rl_grandchild;

    node->height = std::max(height(node->left), height(node->right)) + 1;
    r_child->height = std::max(height(r_child->left), height(r_child->right)) + 1;
    return r_child;
}

NodeAVL* AVLTree::right_rotate(NodeAVL* node) {
    NodeAVL *l_child(node->left);
    NodeAVL* lr_grandchild(l_child->right);

    l_child->right = node;
    node->left = lr_grandchild;

    node->height = std::max(height(node->left), height(node->right)) +1 ;
    l_child->height = std::max(height(l_child->left), height(l_child->right))+ 1;
    return l_child;
}

NodeAVL* AVLTree::remove(NodeAVL* node, const int key) {
    if (node == nullptr)
        return nullptr;

    if (key < node->key) {
        node->left = remove(node->left, key);
    } else if (key > node->key) {
        node->right = remove(node->right, key);
    } else {
        if (node->left == nullptr || node->right == nullptr) {
            size--;
            NodeAVL* n = node->left ? node->left : node->right;
            delete node;
            return n;
        }

        const NodeAVL* min = node->right;
        while (min->left) {
            min = min->left;
        }
        node->key = min->key;
        node->value = min->value;
        node->right = remove(node->right, min->key);
    }

    node->height = std::max(height(node->left), height(node->right)) + 1;

    if (balance_factor(node) > 1) {
        if (balance_factor(node->left) >= 0)
            return right_rotate(node);
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }

    if (balance_factor(node) < -1) {
        if (balance_factor(node->right) <= 0)
            return left_rotate(node);
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }
    return node;
}
int AVLTree::getSize() const {
    return size;
}
void AVLTree::displayTree(const NodeAVL *node) {
    if (node != nullptr) {
        std::cout << "["<<node->key << ": "<<node->value<<"], ";
        displayTree(node->left);
        displayTree(node->right);
    }
}

void AVLTree::display() const {
    displayTree(root);
}

NodeAVL* AVLTree::copy( NodeAVL* node) {
    if (!node) return nullptr;

    NodeAVL* newNodeAVL = new NodeAVL(node->key, node->value);
    newNodeAVL->left = copy(node->left);
    newNodeAVL->right = copy(node->right);
    newNodeAVL->height = node->height;
    return newNodeAVL;
}

void AVLTree::copyFrom(const AVLTree& other) {
    clear(root);
    root = copy(other.root);
    size = other.size;
}
