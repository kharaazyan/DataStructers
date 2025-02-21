#ifndef BST_HPP
#define BST_HPP

#include <cmath>
#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>
#include <initializer_list>
#include <stdexcept>
#include <queue>

template<typename T>
struct node {
    T val;
    node<T>* left;
    node<T>* right;

    node(const T& val_ = T{}, node<T>* left_ = nullptr, node<T>* right_ = nullptr)
        : val(val_), left(left_), right(right_) {}
};

template <typename T>
class binarySearchTree {
    node<T>* root;
    int size_;

    node<T>* insert_(node<T>* tmp, const T& val);
    node<T>* remove_(node<T>* tmp, const T& val);
    node<T>* search_(node<T>* tmp, const T& val) const;
    node<T>* successor_(node<T>* tmp) const;
    node<T>* predecessor_(node<T>* tmp) const ;
    node<T>* getMin_(node<T>* tmp) const ;
    node<T>* getMax_(node<T>* tmp) const ;
    int getHeight_(node<T>* tmp) const; 
    void clear_(node<T>* tmp);

public:
    binarySearchTree() : root(nullptr), size_(0) {}
    ~binarySearchTree() { clear(); }
    void insert(const T& val); 
    void remove(const T& val);
    bool search(const T& val) const;
    void clear();
    int getHeight(){ return getHeight_(root); }
    int size() const { return size_; }
    bool empty() const { return size_ == 0; }
    void inOrderDisplay() const;
    void display() const;
};

template<typename T>
void binarySearchTree<T>::insert(const T& val){
    root = insert_(root, val);
    ++size_;
}

template<typename T>
node<T>* binarySearchTree<T>::  insert_(node<T>* tmp, const T& val){
    if(tmp == nullptr) return new node<T>(val);
    if(val < tmp->val) tmp->left = insert_(tmp->left, val);
    else if(val > tmp->val) tmp->right = insert_(tmp->right, val);
    return tmp;
}

template<typename T>
void binarySearchTree<T>::remove(const T& val){
    if(search(val)){
        root = remove_(root, val);
        --size_;
    }
}

template<typename T>
node<T>* binarySearchTree<T>::remove_(node<T>* tmp, const T& val){
    if(!tmp) return nullptr;
    if(val < tmp->val) tmp->left = remove_(tmp->left, val);
    else if(val > tmp->val) tmp->right = remove_(tmp->right, val);
    else{
        node<T>* temp{};
        if(!tmp->left){
            temp = tmp->right;
            delete tmp;
            return temp;
        }
        if(!tmp->right){
            temp = tmp->left;
            delete tmp;
            return temp;
        }
        temp = successor_(tmp);
        tmp->val = temp->val;
        tmp->right = remove_(tmp->right, temp->val);
    }
    return tmp;
}

template<typename T>
bool binarySearchTree<T>::search(const T& val) const{
    return static_cast<bool>(search_(root, val));
}

template<typename T>
node<T>* binarySearchTree<T>::search_(node<T>* tmp, const T& val) const{
    if (tmp == nullptr) return nullptr;
    if (tmp->val == val) return tmp;
    if (tmp->val < val) return search_(tmp->right, val);  
    else return search_(tmp->left, val);
}

template<typename T>
node<T>* binarySearchTree<T>::getMin_(node<T>* tmp) const{
    if (!tmp) return nullptr;             
    if (tmp->left == nullptr) return tmp;                     
    return getMin_(tmp->left);           
}

template<typename T>
node<T>* binarySearchTree<T>::getMax_(node<T>* tmp) const{
    if (!tmp) return nullptr;             
    if (tmp->right == nullptr) return tmp;                     
    return getMax_(tmp->right);           
}

template<typename T>
int binarySearchTree<T>::getHeight_(node<T>* tmp) const{
    if(!tmp) return -1;
    return std::max(getHeight_(tmp->left), getHeight_(tmp->right)) + 1;
}

template<typename T>
void binarySearchTree<T>::clear() {
    clear_(root);
    size_ = 0;
    root = nullptr;
}

template<typename T>
void binarySearchTree<T>::clear_(node<T>* tmp) {
    if(!tmp) return;
    clear_(tmp->left);
    clear_(tmp->right);
    delete tmp;
}

template<typename T>
node<T>* binarySearchTree<T>::successor_(node<T>* tmp) const{
    if(!tmp) return nullptr;
    if(tmp->right) return getMin_(tmp->right);
    node<T>*prev = nullptr;
    node<T>* ancestor = root;
    while(ancestor != tmp){
        if(ancestor->val > tmp->val){
            prev = ancestor;
            ancestor = ancestor->left;
        }
        else ancestor = ancestor->right;
    }
    return prev;
}

template<typename T>
node<T>* binarySearchTree<T>::predecessor_(node<T>* tmp) const{
    if(!tmp) return nullptr;
    if(tmp->left) return getMax_(tmp->left);
    node<T>* prev = nullptr;
    node<T>* ancestor = root;
    while(ancestor != tmp){
        if(ancestor->val < tmp->val){
            prev = ancestor;
            ancestor = ancestor->right;
        }
        else ancestor = ancestor->left;
    }
    return prev;
}

template<typename T>
void binarySearchTree<T>::display() const {
    if (empty()) {
        std::cout << "Tree empty" << std::endl;
        return;
    }

    int h = getHeight_(root);
    int levels = h + 1;

    std::queue<node<T>*> q;
    q.push(root);

    for (int level = 0; level < levels; ++level) {
        int initialSpaces = std::pow(2, levels - level) - 1;
        int betweenSpaces = std::pow(2, levels - level + 1) - 1;

        for (int s = 0; s < initialSpaces; ++s)
            std::cout << " ";

        int levelNodeCount = q.size();

        std::vector<node<T>*> nextLevel;

        for (int i = 0; i < levelNodeCount; ++i) {
            node<T>* curr = q.front();
            q.pop();

            if (curr) {
                std::cout << curr->val;
                nextLevel.push_back(curr->left);
                nextLevel.push_back(curr->right);
            } else {
                std::cout << " ";  
                nextLevel.push_back(nullptr);
                nextLevel.push_back(nullptr);
            }

            for (int s = 0; s < betweenSpaces; ++s) std::cout << " ";
        }
        std::cout << std::endl;

        bool allNull = true;
        for (node<T>* n : nextLevel) {
            if (n != nullptr) { allNull = false; break; }
        }
        if (allNull)
            break;

        for (node<T>* n : nextLevel)
            q.push(n);
    }
}


template<typename T>
void binarySearchTree<T>::inOrderDisplay() const {
    std::function<void(node<T>*)> inOrder = [&](node<T>* cur) {
        if (!cur) return;
        inOrder(cur->left);
        std::cout << cur->val << " ";
        inOrder(cur->right);
    };
    inOrder(root);
    std::cout << std::endl;
}

#endif