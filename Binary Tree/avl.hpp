#ifndef AVL_HPP
#define AVL_HPP

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
class avl {
    node<T>* root;
    int size_;

    node<T>* insert_(node<T>* tmp, const T& val);
    node<T>* remove_(node<T>* tmp, const T& val);
    node<T>* rotate_(node<T>* tmp);
    node<T>* search_(node<T>* tmp, const T& val) const;
    node<T>* successor_(node<T>* tmp) const;
    node<T>* predecessor_(node<T>* tmp) const ;
    node<T>* getMin_(node<T>* tmp) const ;
    node<T>* getMax_(node<T>* tmp) const ;
    node<T>* rightRotation_(node<T>* tmp);
    node<T>* leftRotation_(node<T>* tmp);
    int balanceFactor_(node<T>* tmp) const;
    int getHeight_(node<T>* tmp) const; 
    void clear_(node<T>* tmp);

public:
    avl() : root(nullptr), size_(0) {}
    ~avl() { clear(); }
    void insert(const T& val); 
    void remove(const T& val);
    bool search(const T& val) const;
    void clear();
    int getHeight(){ return getHeight_(root); }
    int size() const { return size_; }
    bool empty() const { return size_ == 0; }
    void display() const;
};

template<typename T>
void avl<T>::insert(const T& val){
    root = insert_(root, val);
    ++size_;
}

template<typename T>
node<T>* avl<T>::insert_(node<T>* tmp, const T& val){
    if(!tmp) return new node<T>(val);
    if(val > tmp->val) tmp->right = insert_(tmp->right, val);
    else tmp->left = insert_(tmp->left, val);
    return rotate_(tmp);
}

template<typename T>
node<T>* avl<T>::rotate_(node<T>* tmp) {
    int balance = balanceFactor_(tmp);
    if(std::abs(balance) <= 1) return tmp;
    if(balance > 1){
        int child = balanceFactor_(tmp->left);
        if(child >= 0) return rightRotation_(tmp);
        else{
            tmp->left = leftRotation_(tmp->left);
            return rightRotation_(tmp);
        }
    }
    else{
        int child = balanceFactor_(tmp->right);
        if(child <= 0) return leftRotation_(tmp);
        else{
            tmp->right = rightRotation_(tmp->right);
            return leftRotation_(tmp);
        }
    }
}

template<typename T>
void avl<T>::remove(const T& val){
    if(search(val)){
        root = remove_(root, val);
        --size_;
    }
}

template<typename T>
node<T>* avl<T>::remove_(node<T>* tmp, const T& val){
    if(!tmp) return nullptr;
    if(val < tmp->val) tmp->left = remove_(tmp->left, val);
    else if(val > tmp->val) tmp->right = remove_(tmp->right, val);
    else{
        node<T>* temp{};
        if(!tmp->left){
            temp = tmp->right;
            delete tmp;
            return rotate_(temp);
        }
        if(!tmp->right){
            temp = tmp->left;
            delete tmp;
            return rotate_(temp);
        }
        temp = successor_(tmp);
        tmp->val = temp->val;
        tmp->right = remove_(tmp->right, temp->val);
    }
    return rotate_(tmp);
}

template<typename T>
node<T>* avl<T>::rightRotation_(node<T>* tmp) {
    node<T>* x = tmp->left;
    tmp->left = x->right;
    x->right = tmp;
    return x; 
}

template<typename T>
node<T>* avl<T>::leftRotation_(node<T>* tmp) {
    node<T>* y = tmp->right;
    tmp->right = y->left;
    y->left = tmp;
    return y; 
}

template<typename T>
int avl<T>::balanceFactor_(node<T>* tmp) const {
    if (!tmp) return 0;
    return getHeight_(tmp->left) - getHeight_(tmp->right);
}

template<typename T>
int avl<T>::getHeight_(node<T>* tmp) const{
    if(!tmp) return -1;
    return std::max(getHeight_(tmp->left), getHeight_(tmp->right)) + 1;
}

template<typename T>
bool avl<T>::search(const T& val) const{
    return static_cast<bool>(search_(root, val));
}

template<typename T>
node<T>* avl<T>::search_(node<T>* tmp, const T& val) const{
    if (tmp == nullptr) return nullptr;
    if (tmp->val == val) return tmp;
    if (tmp->val < val) return search_(tmp->right, val);  
    else return search_(tmp->left, val);
}

template<typename T>
node<T>* avl<T>::getMin_(node<T>* tmp) const{
    if (!tmp) return nullptr;             
    if (tmp->left == nullptr) return tmp;                     
    return getMin_(tmp->left);           
}

template<typename T>
node<T>* avl<T>::getMax_(node<T>* tmp) const{
    if (!tmp) return nullptr;             
    if (tmp->right == nullptr) return tmp;                     
    return getMax_(tmp->right);           
}

template<typename T>
void avl<T>::clear() {
    clear_(root);
    size_ = 0;
    root = nullptr;
}

template<typename T>
void avl<T>::clear_(node<T>* tmp) {
    if(!tmp) return;
    clear_(tmp->left);
    clear_(tmp->right);
    delete tmp;
}

template<typename T>
node<T>* avl<T>::successor_(node<T>* tmp) const{
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
node<T>* avl<T>::predecessor_(node<T>* tmp) const{
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
void avl<T>::display() const {
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

#endif