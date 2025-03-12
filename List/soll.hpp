#ifndef SOLL_HPP
#define SOLL_HPP

#include <initializer_list>
#include <utility>
#include <stdexcept> 
#include <iostream>

template<typename T>
struct node {
    T value;         
    node<T>* next;  
    node<T>* prev;  
    node<T>* left;   
    node<T>* right;

    node()
        : value{}, next(nullptr), prev(nullptr), left(nullptr), right(nullptr)
    {}

    node(const T& value_,
         node<T>* next_ = nullptr,
         node<T>* prev_ = nullptr,
         node<T>* left_ = nullptr,
         node<T>* right_ = nullptr)
        : value(value_), next(next_), prev(prev_), left(left_), right(right_)
    {}

    node(T&& value_,
         node<T>* next_ = nullptr,
         node<T>* prev_ = nullptr,
         node<T>* left_ = nullptr,
         node<T>* right_ = nullptr)
        : value(std::move(value_)), next(next_), prev(prev_), left(left_), right(right_)
    {}
};

template <typename T>
class solist {
public:
    using value_type      = T;
    using size_type       = std::size_t;
    using reference       = T&;
    using const_reference = const T&;
    using pointer         = T*;
    using const_pointer   = const T*;

private:
    node<T>* head;  
    node<T>* tail;  
    node<T>* low;   
    node<T>* high; 
    size_type size_;

private:
    void unlink_from_sorted(node<T>* n);
    void link_into_sorted(node<T>* new_node);

public:
    solist() noexcept;
    solist(const solist& other);            
    solist(solist&& other) noexcept;
    solist(std::initializer_list<T> init);
    ~solist();

    solist& operator=(const solist& other);
    solist& operator=(solist&& other) noexcept;

    void push_front(const_reference value);
    void push_front(T&& value);
    void push_back(const_reference value);
    void push_back(T&& value);

    void pop_front();
    void pop_back();

    node<T>* insert(node<T>* pos, const_reference value);
    node<T>* insert(node<T>* pos, T&& value);

    void clear();
    bool empty() const     { return size_ == 0; }
    size_type size() const { return size_; }

    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    void display() const;

    node<T>* begin() { return head; }
    node<T>* end()   { return nullptr; }

    const node<T>* begin() const { return head; }
    const node<T>* end()   const { return nullptr; }
};

template <typename T>
void solist<T>::unlink_from_sorted(node<T>* n)
{
    if (!n) return;
    node<T>* L = n->left;
    node<T>* R = n->right;

    if (L) L->right = R;
    else low = R;
    
    if (R) R->left = L;
    else high = L;

    n->left  = nullptr;
    n->right = nullptr;
}

template <typename T>
void solist<T>::link_into_sorted(node<T>* new_node)
{
    if (!new_node) return;

    if (!low && !high) {
        low  = new_node;
        high = new_node;
        return;
    }

    node<T>* tmp = low;
    while (tmp && new_node->value >= tmp->value) {
        tmp = tmp->right;
    }

    if (tmp) {
        // Вставляем перед tmp
        new_node->right = tmp;
        new_node->left  = tmp->left;
        if (tmp->left) {
            tmp->left->right = new_node;
        } 
        else low = new_node;
        
        tmp->left = new_node;
    } else {
        new_node->right = nullptr;
        new_node->left  = high;
        if (high) {
            high->right = new_node;
        }
        high = new_node;
    }
    if (new_node->value < low->value) low = new_node;
    
}


template <typename T>
solist<T>::solist() noexcept
    : head(nullptr),
      tail(nullptr),
      low(nullptr),
      high(nullptr),
      size_(0)
{}

template<typename T>
solist<T>::~solist(){
    clear();
}

template <typename T>
solist<T>::solist(const solist& other)
    : head(nullptr), tail(nullptr), low(nullptr), high(nullptr), size_(0)
{
    node<T>* tmp = other.head;
    while(tmp){
        push_back(tmp->value);
        tmp = tmp->next;
    }
}

template <typename T>
solist<T>::solist(solist&& other) noexcept
    : head(other.head), tail(other.tail),
      low(other.low), high(other.high),
      size_(other.size_)
{
    other.head  = nullptr;
    other.tail  = nullptr;
    other.low   = nullptr;
    other.high  = nullptr;
    other.size_ = 0;
}

template <typename T>
solist<T>::solist(std::initializer_list<T> init)
    : head(nullptr), tail(nullptr), low(nullptr), high(nullptr), size_(0)
{
    for(const auto& val : init){
        push_back(val);
    }
}

template <typename T>
solist<T>& solist<T>::operator=(const solist<T>& other) {
    if (this != &other) {
        clear();
        node<T>* tmp = other.head;
        while(tmp){
            push_back(tmp->value);
            tmp = tmp->next;
        }
    }
    return *this;
}

template <typename T>
solist<T>& solist<T>::operator=(solist<T>&& other) noexcept {
    if (this != &other) {
        clear();
        head  = other.head;
        tail  = other.tail;
        low   = other.low;
        high  = other.high;
        size_ = other.size_;

        other.head  = nullptr;
        other.tail  = nullptr;
        other.low   = nullptr;
        other.high  = nullptr;
        other.size_ = 0;
    }
    return *this;
}


template<typename T>
void solist<T>::push_front(const_reference value){
    node<T>* new_node = new node<T>(value);

    if(!head){
        head = tail = new_node;
    }
    else {
        new_node->next = head;
        head->prev     = new_node;
        head           = new_node;
    }
    link_into_sorted(new_node);
    ++size_;
}


template<typename T>
void solist<T>::push_front(T&& value){
    node<T>* new_node = new node<T>(std::move(value));

    if(!head){
        head = tail = new_node;
    }
    else {
        new_node->next = head;
        head->prev     = new_node;
        head           = new_node;
    }
    link_into_sorted(new_node);
    ++size_;
}

template<typename T>
void solist<T>::push_back(const_reference value){
    node<T>* new_node = new node<T>(value);

    if(!head){
        head = tail = new_node;
    }
    else {
        new_node->prev  = tail;
        tail->next      = new_node;
        tail            = new_node;
    }
    link_into_sorted(new_node);
    ++size_;
}

template<typename T>
void solist<T>::push_back(T&& value){
    node<T>* new_node = new node<T>(std::move(value));

    if(!head){
        head = tail = new_node;
    }
    else {
        new_node->prev = tail;
        tail->next     = new_node;
        tail           = new_node;
    }
    link_into_sorted(new_node);
    ++size_;
}


template<typename T>
void solist<T>::pop_front(){
    if (!head) return; 

    node<T>* old_head = head;
    unlink_from_sorted(old_head);

    if (head == tail) {
        head = tail = nullptr;
    } else {
        head = head->next;
        if (head) head->prev = nullptr;
    }
    delete old_head;
    --size_;
    if (size_ == 0) {
        low  = nullptr;
        high = nullptr;
    }
}


template<typename T>
void solist<T>::pop_back(){
    if (!tail) return; 

    node<T>* old_tail = tail;
    unlink_from_sorted(old_tail);

    if (head == tail) {
        head = tail = nullptr;
    } else {
        tail = tail->prev;
        if (tail) tail->next = nullptr;
    }
    delete old_tail;
    --size_;
    if (size_ == 0) {
        low  = nullptr;
        high = nullptr;
    }
}


template<typename T>
node<T>* solist<T>::insert(node<T>* pos, const_reference value){
    if(!pos) {
        push_back(value);
        return tail;
    }
    if(pos == head) {
        push_front(value);
        return head;
    }

    node<T>* new_node = new node<T>(value, pos, pos->prev);
    pos->prev->next   = new_node;
    pos->prev         = new_node;

    link_into_sorted(new_node);

    ++size_;
    return new_node;
}


template<typename T>
node<T>* solist<T>::insert(node<T>* pos, T&& value){
    if(!pos) {
        push_back(std::move(value));
        return tail;
    }
    if(pos == head) {
        push_front(std::move(value));
        return head;
    }

    node<T>* new_node = new node<T>(std::move(value), pos, pos->prev);
    pos->prev->next = new_node;
    pos->prev       = new_node;

    link_into_sorted(new_node);

    ++size_;
    return new_node;
}


template <typename T>
void solist<T>::clear(){
    node<T>* current = head;
    while(current){
        node<T>* next_node = current->next;
        delete current;
        current = next_node;
    }
    head = tail = low = high = nullptr;
    size_ = 0;
}


template<typename T>
typename solist<T>::reference solist<T>::front() {
    if(empty()){
        throw std::out_of_range("Попытка доступа к первому элементу пустого списка.");
    }
    return head->value;
}

template<typename T>
typename solist<T>::const_reference solist<T>::front() const {
    if(empty()){
        throw std::out_of_range("Попытка доступа к первому элементу пустого списка.");
    }
    return head->value;
}

template<typename T>
typename solist<T>::reference solist<T>::back() {
    if(empty()){
        throw std::out_of_range("Попытка доступа к последнему элементу пустого списка.");
    }
    return tail->value;
}

template<typename T>
typename solist<T>::const_reference solist<T>::back() const {
    if(empty()){
        throw std::out_of_range("Попытка доступа к последнему элементу пустого списка.");
    }
    return tail->value;
}


template<typename T>
void solist<T>::display() const {
    node<T>* current = low;
    while (current) {
        std::cout << current->value << " -> ";
        current = current->right;
    }
    std::cout << "nullptr" << std::endl;
}

#endif //SOLL_HPP
