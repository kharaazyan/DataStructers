#ifndef READBLACKTREE_HPP
#define READBLACKTREE_HPP

#include <utility>
#include <iostream>
#include <algorithm>

enum class Color{BLACK, RED};

template <typename T>
struct node {
    T val;
    Color color;
    node<T>* parent;
    node<T>* left;
    node<T>* right;

    node(node<T>* nil, const T& val_ = T{})
        : val(val_), color(Color::RED), left(nil), right(nil), parent(nil) {}
};

template <typename T>
class redBlackTree{
    node<T>* root;
    node<T>* nil;
    int size_;

    void leftRotate(node<T>* tmp);
    int getHeight_(node<T>* tmp) const; 
    void clear_(node<T>* tmp);
    
    public:
    redBlackTree();
    ~redBlackTree();
    void insert(const T& val); 
    void remove(const T& val);
    bool search(const T& val) const;
    void clear();
    void display() const;
    int size(){ return size_; }
    bool empty(){ return size_ == 0; }
    int getHeight(){ return getHeight_(root); }
};

template <typename T>
redBlackTree<T>::redBlackTree() : size_(0){
    nil = new node<T>(nullptr);
    nil->color = Color::BLACK;
    root = nil;
}

template <typename T>
redBlackTree<T>::~redBlackTree(){
    clear();
    delete nil;
}

template <typename T>
void redBlackTree<T>::leftRotate(node<T>* x) {
    node<T>* y = x->right;
    x->right = y->left;
    if (y->left != nil) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nil) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}   

template<typename T>
int redBlackTree<T>::getHeight_(node<T>* tmp) const{
    if (tmp == nil) return -1;
    return std::max(getHeight_(tmp->left), getHeight_(tmp->right)) + 1;
}

template<typename T>
void redBlackTree<T>::clear() {
    clear_(root);
    size_ = 0;
    root = nullptr;
}

template<typename T>
void redBlackTree<T>::clear_(node<T>* tmp) {
    if(!tmp) return;
    if(tmp->left != nil) clear_(tmp->left);
    if(tmp->left != nil) clear_(tmp->right);
    delete tmp;
}

template<typename T>
void redBlackTree<T>::display() const {
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
                char c = (curr->color == Color::RED) ? 'R' : 'B';
                std::cout << curr->val << "(" << c << ")";
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