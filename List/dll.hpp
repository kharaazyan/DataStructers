#ifndef DLL_HPP
#define DLL_HPP

#include <initializer_list>
#include <utility>
#include <stdexcept> 
#include <iostream>

template<typename T>
struct node{
    T value;
    node<T>* next; 
    node<T>* prev; 

    node() : value{}, next(nullptr), prev(nullptr){}
    node(const T& value_, node<T>* next = nullptr, node<T>* prev = nullptr) 
        : value(value_), next(next), prev(prev){}
    node(T&& value_, node<T>* next = nullptr, node<T>* prev = nullptr) 
        : value(std::move(value_)), next(next), prev(prev){}
};

template <typename T>
class dlist {
public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = node<T>*;
    using const_iterator = const node<T>*;

private:
    node<T>* head;
    node<T>* tail;
    size_type size_;

public:
    dlist() noexcept;
    dlist(const dlist& other);            
    dlist(dlist&& other) noexcept;
    dlist(std::initializer_list<T> init);
    ~dlist();

    dlist& operator=(const dlist& other);
    dlist& operator=(dlist&& other) noexcept;

    void push_front(const_reference value);
    void push_front(T&& value);
    void push_back(const_reference value);
    void push_back(T&& value);
    void pop_front();
    void pop_back();
    iterator insert(iterator pos, const_reference value);
    iterator insert(iterator pos, T&& value);
    void clear();
    bool empty() const { return size_ == 0; }
    size_type size() const {return size_; }
    void display() const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    iterator begin() { return head; }
    const_iterator begin() const { return head; }
    iterator end() { return nullptr; } 
    const_iterator end() const { return nullptr; }
};


template <typename T>
dlist<T>::dlist() noexcept : head(nullptr), tail(nullptr), size_(0){}

template<typename T>
dlist<T>::~dlist(){
    clear();
}

template <typename T>
dlist<T>::dlist(const dlist& other) : head(nullptr), tail(nullptr), size_(0){
    node<T>* tmp = other.head;
    while(tmp){
        push_back(tmp->value);
        tmp = tmp->next;
    }
}

template <typename T>
dlist<T>::dlist(dlist&& other) noexcept : head(other.head), tail(other.tail), size_(other.size_){
    other.head = nullptr;
    other.tail = nullptr;
    other.size_ = 0;
}

template <typename T>
dlist<T>::dlist(std::initializer_list<T> init) : head(nullptr), tail(nullptr), size_(0){
    for(auto &i : init){
        push_back(i);
    }
}

template <typename T>
dlist<T>& dlist<T>::operator=(const dlist<T>& other) {
    if (this != &other) { 
        clear(); 

        node<T>* tmp = other.head;
        while (tmp) {
            push_back(tmp->value); 
            tmp = tmp->next;
        }
    }
    return *this;
}

template <typename T>
dlist<T>& dlist<T>::operator=(dlist<T>&& other) noexcept {
    if (this != &other) { 
        clear(); 

        head = other.head;
        tail = other.tail;
        size_ = other.size_;

        other.head = nullptr;
        other.tail = nullptr;
        other.size_ = 0;
    }
    return *this;
}

template<typename T>
void dlist<T>::push_front(const_reference value){
    if(!head){
        head = tail = new node<T>(value, nullptr, nullptr);
    }
    else if (head == tail){
        head = new node<T>(value, tail, nullptr);
        tail->prev = head;
    }
    else{
        node<T>* tmp = new node<T>(value, head, nullptr);
        head->prev = tmp;
        head = tmp;
    }
    ++size_;
}

template<typename T>
void dlist<T>::push_front(T&& value){
    if(!head){
        head = tail = new node<T>(std::move(value), nullptr, nullptr);
    }
    else if (head == tail){
        head = new node<T>(std::move(value), tail, nullptr);
        tail->prev = head;
    }
    else{
        node<T>* tmp = new node<T>(std::move(value), head, nullptr);
        head->prev = tmp;
        head = tmp;
    }
    ++size_;
}

template<typename T>
void dlist<T>::push_back(const_reference value){
    if(!head){
        head = tail = new node<T>(value, nullptr, nullptr);
    }
    else if (head == tail){
        tail = new node<T>(value, nullptr, head);
        head->next = tail;
    }
    else{
        node<T>* tmp = new node<T>(value, nullptr, tail);
        tail->next = tmp;
        tail = tmp;
    }
    ++size_;
}

template<typename T>
void dlist<T>::push_back(T&& value){
    if(!head){
        head = tail = new node<T>(std::move(value), nullptr, nullptr);
    }
    else if (head == tail){
        tail = new node<T>(std::move(value), nullptr, head);
        head->next = tail;
    }
    else{
        node<T>* tmp = new node<T>(std::move(value), nullptr, tail);
        tail->next = tmp;
        tail = tmp;
    }
    ++size_;
}

template<typename T>
void dlist<T>::pop_back(){
    if (!tail) return;

    node<T>* old_tail = tail; 

    if (tail->prev) {
        tail = tail->prev;
        tail->next = nullptr;
    } else {
        head = nullptr;
        tail = nullptr;
    }
    delete old_tail; 
    --size_;
}

template<typename T>
void dlist<T>::pop_front(){
    if (!head) return;

    node<T>* old_head = head; 

    if (head->next) {
        head = head->next;
        head->prev = nullptr;
    } else {
        head = nullptr;
        tail = nullptr;
    }
    delete old_head; 
    --size_;
}

template <typename T>
void dlist<T>::clear(){
    while(!empty()) pop_back();
}

template<typename T>
void dlist<T>::display() const {
    node<T>* current = head;
    while (current) {
        std::cout << current->value << " -> ";
        current = current->next;
    }
    std::cout << "nullptr" << std::endl;
}

template<typename T>
typename dlist<T>::iterator dlist<T>::insert(iterator pos, const_reference value){
    if(pos == head){
        push_front(value);
        return head;
    }
    if(pos == nullptr){ 
        push_back(value);
        return tail;
    }
    node<T>* current = head;
    while(current != nullptr && current != pos){
        current = current->next;
    }
    if(current == nullptr){
        throw std::out_of_range("Итератор вне диапазона списка.");
    }
    node<T>* new_node = new node<T>(value, current, current->prev);
    if(current->prev){
        current->prev->next = new_node;
    }
    current->prev = new_node;
    ++size_;
    return new_node;
}

template<typename T>
typename dlist<T>::iterator dlist<T>::insert(iterator pos, T&& value){
    if(pos == head){
        push_front(std::move(value));
        return head;
    }
    if(pos == nullptr){ 
        push_back(std::move(value));
        return tail;
    }
    node<T>* current = head;
    while(current != nullptr && current != pos){
        current = current->next;
    }
    if(current == nullptr){
        throw std::out_of_range("Итератор вне диапазона списка.");
    }
    node<T>* new_node = new node<T>(std::move(value), current, current->prev);
    if(current->prev){
        current->prev->next = new_node;
    }
    current->prev = new_node;
    ++size_;
    return new_node;
}

template<typename T>
typename dlist<T>::reference dlist<T>::front() {
    if(empty()){
        throw std::out_of_range("Попытка доступа к первому элементу пустого списка.");
    }
    return head->value;
}

template<typename T>
typename dlist<T>::const_reference dlist<T>::front() const {
    if(empty()){
        throw std::out_of_range("Попытка доступа к первому элементу пустого списка.");
    }
    return head->value;
}

template<typename T>
typename dlist<T>::reference dlist<T>::back() {
    if(empty()){
        throw std::out_of_range("Попытка доступа к последнему элементу пустого списка.");
    }
    return tail->value;
}

template<typename T>
typename dlist<T>::const_reference dlist<T>::back() const {
    if(empty()){
        throw std::out_of_range("Попытка доступа к последнему элементу пустого списка.");
    }
    return tail->value;
}


#endif //DLL_HPP
