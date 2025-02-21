#ifndef SLL_HPP
#define SLL_HPP

#include <initializer_list>
#include <utility>
#include <stdexcept> 
#include <iostream>

template<typename T>
struct node{
    T value;
    node<T>* next; 

    node() : value{}, next(nullptr){}
    node(const T& value_) : value(value_), next(nullptr){}
    node(T&& value_) : value(std::move(value_)), next(nullptr){}
};

template <typename T>
class list {
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
    size_type size_;

public:
    list();
    list(const list& other);            
    list(list&& other) noexcept;
    list(std::initializer_list<T> init);
    ~list();

    list& operator=(const list& other);
    list& operator=(list&& other) noexcept;

    void push_front(const_reference value);
    void push_front(T&& value);
    void push_back(const_reference value);
    void push_back(T&& value);
    void pop_front();
    void pop_back();
    iterator insert(iterator pos, const_reference value);
    iterator insert(iterator pos, T&& value);
    void remove(const T& value);
    iterator find(const T& value);
    const_iterator find(const T& value) const;
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
list<T>::list() : head(nullptr), size_(0){}

template<typename T>
list<T>::list(const list& other) : head(nullptr), size_(0){
    node<T>* current = other.head;
    while(current != nullptr){
        push_back(current->value);
        current = current->next;
    }
}

template<typename T>
list<T>::list(list&& other) noexcept : head(other.head), size_(other.size_) {
    other.head = nullptr;
    other.size_ = 0;
}

template<typename T>
list<T>::list(std::initializer_list<T> init) : head(nullptr), size_(0){
    for(auto &i : init){
        push_back(i);
    }
}

template<typename T>
list<T>::~list(){
    clear();
}

template<typename T>
list<T>& list<T>::operator=(const list& other){
    if(this != &other){
        clear();
        node<T>* current = other.head;
        while(current != nullptr){
            push_back(current->value);
            current = current->next;
        }
    }
    return *this;
}

template<typename T>
list<T>& list<T>::operator=(list&& other) noexcept{
    if(this != &other){
        clear();
        head = other.head;
        size_ = other.size_;
        other.head = nullptr;
        other.size_ = 0;
    }
    return *this;
}

template<typename T>
void list<T>::push_front(const_reference value){
    node<T>* new_node = new node<T>(value);
    new_node->next = head;
    head = new_node;
    ++size_;
}

template<typename T>
void list<T>::push_front(T&& value){
    node<T>* new_node = new node<T>(std::move(value));
    new_node->next = head;
    head = new_node;
    ++size_;
}

template<typename T>
void list<T>::push_back(const_reference value){
    if(empty()){
        head = new node<T>(value);
    }
    else{
        node<T>* current = head;
        while (current->next != nullptr){
            current = current->next;
        }
        current->next = new node<T>(value);
    }
    ++size_;
}

template<typename T>
void list<T>::push_back(T&& value){
    if(empty()){
        head = new node<T>(std::move(value));
    }
    else{
        node<T>* current = head;
        while (current->next != nullptr){
            current = current->next;
        }
        current->next = new node<T>(std::move(value));
    }
    ++size_;
}

template<typename T>
void list<T>::pop_front(){
    if(empty()) return;
    node<T>* tmp = head;
    head = head->next;
    delete tmp;
    --size_;
}

template<typename T>
void list<T>::pop_back(){
    if(empty()) return;
    if(head->next == nullptr){
        delete head;
        head = nullptr;
    }
    else{
        node<T>* current = head;
        while(current->next->next != nullptr){
            current = current->next;
        }
        delete current->next;
        current->next = nullptr;
    }
    --size_;
}

template<typename T>
typename list<T>::iterator list<T>::insert(iterator pos, const_reference value){
    if(pos == head){
        push_front(value);
        return head;
    }
    node<T>* current = head;
    while(current != nullptr && current->next != pos){
        current = current->next;
    }
    if(current == nullptr){
        throw std::out_of_range("Итератор вне диапазона списка.");
    }
    node<T>* new_node = new node<T>(value);
    new_node->next = current->next;
    current->next = new_node;
    ++size_;
    return new_node;
}

template<typename T>
typename list<T>::iterator list<T>::insert(iterator pos, T&& value){
    if(pos == head){
        push_front(std::move(value));
        return head;
    }
    node<T>* current = head;
    while(current != nullptr && current->next != pos){
        current = current->next;
    }
    if(current == nullptr){
        throw std::out_of_range("Итератор вне диапазона списка.");
    }
    node<T>* new_node = new node<T>(std::move(value));
    new_node->next = current->next;
    current->next = new_node;
    ++size_;
    return new_node;
}

template<typename T>
void list<T>::remove(const T& value){
    if(empty()) return;

    while(head != nullptr && head->value == value){
        node<T>* tmp = head;
        head = head->next;
        delete tmp;
        --size_;
    }

    if(head == nullptr) return;

    node<T>* current = head;
    while(current->next != nullptr){
        if(current->next->value == value){
            node<T>* tmp = current->next;
            current->next = current->next->next;
            delete tmp;
            --size_;
        }
        else{
            current = current->next;
        }
    }
}

template<typename T>
typename list<T>::iterator list<T>::find(const T& value){
    node<T>* tmp = head;
    while(tmp != nullptr){
        if(tmp->value == value) return tmp;
        else tmp = tmp->next;
    }
    return nullptr;
}

template<typename T>
typename list<T>::const_iterator list<T>::find(const T& value) const{
    node<T>* tmp = head;
    while(tmp != nullptr){
        if(tmp->value == value) return tmp;
        else tmp = tmp->next;
    }
    return nullptr;
}

template<typename T>
void list<T>::clear(){
    while(!empty()){
        pop_front();
    }
}

template<typename T>
void list<T>::display() const{
    node<T>* current = head;
    while(current != nullptr){
        std::cout << current->value << " -> ";
        current = current->next;
    }
    std::cout << "nullptr" << std::endl;
}

template<typename T>
typename list<T>::reference list<T>::front(){
    if(empty()){
        throw std::out_of_range("Список пуст. Невозможно получить первый элемент.");
    }
    return head->value;
}

template<typename T>
typename list<T>::const_reference list<T>::front() const{
    if(empty()){
        throw std::out_of_range("Список пуст. Невозможно получить первый элемент.");
    }
    return head->value;
}

template<typename T>
typename list<T>::reference list<T>::back(){
    if(empty()){
        throw std::out_of_range("Список пуст. Невозможно получить последний элемент.");
    }
    node<T>* current = head;
    while(current->next != nullptr){
        current = current->next;
    }
    return current->value;
}

template<typename T>
typename list<T>::const_reference list<T>::back() const{
    if(empty()){
        throw std::out_of_range("Список пуст. Невозможно получить последний элемент.");
    }
    node<T>* current = head;
    while(current->next != nullptr){
        current = current->next;
    }
    return current->value;
}


#endif // SLL_HPP
