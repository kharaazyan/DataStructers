#ifndef BINARY_HEAP
#define BINARY_HEAP

#include <cmath>  
#include <vector>
#include <utility>
#include <iostream>
#include <initializer_list>
#include <stdexcept>

template<typename T>
class binaryHeapMax{
    int size_;
    std::vector<T> data;
public:
    binaryHeapMax() noexcept;
    binaryHeapMax(const binaryHeapMax<T>& other);
    binaryHeapMax(std::initializer_list<T> init);
    binaryHeapMax<T>& operator=(const binaryHeapMax<T>& other);

    
    void push(const T& value);
    const T& top() const;
    void pop();
    void clear() { data.clear(); size_ = 0;}
    void display() const;
    size_t size(){ return size_; }
    bool empty() const { return size_ == 0; }
private:
    int getLeft(int i) const;
    int getRight(int i) const;
    int getParent(int i) const;
    void build();
    int start() { return (size_ / 2) - 1;}
    void heapify(int i, int size);
};

template<typename T>
binaryHeapMax<T>::binaryHeapMax() noexcept : size_(0){}

template<typename T>
binaryHeapMax<T>::binaryHeapMax(const binaryHeapMax<T>& other){
    data = other.data;
    size_ = data.size();
}

template<typename T>
binaryHeapMax<T>::binaryHeapMax(std::initializer_list<T> init){
    for(auto &i : init){
        data.push_back(std::move(i));
    }
    size_ = data.size();
    build();
}
template<typename T>
binaryHeapMax<T>& binaryHeapMax<T>::operator=(const binaryHeapMax<T>& other){
    if(this == &other) return *this;
    data.clear();
    data = other.data;
    size_ = data.size();
    return *this;
}

template<typename T>
const T& binaryHeapMax<T>::top() const { 
    if(empty()){
        throw std::out_of_range("Empty heap");
    }
    return data[0]; 
};

template<typename T>
void binaryHeapMax<T>::pop() { 
    if(empty()){
        throw std::out_of_range("Empty heap");
    }
    T tmp = data[0];
    data[0] = data[size_ - 1];
    data.pop_back();
    if(--size_ > 0) heapify(0, size_);
    return tmp; 
};

template<typename T>
void binaryHeapMax<T>::push(const T& value) { 
    data.push_back(value);
    ++size_;
    int i = size_ - 1;
    while(i > 0){
        int parent = getParent(i);
        if(parent == -1) break;
        if(data[parent] < data[i]){
            std::swap(data[parent], data[i]);
            i = parent;
        }
        else break;
    }
};

template<typename T>
void binaryHeapMax<T>::heapify(int i, int size){
    int max;
    int left = getLeft(i);
    int right = getRight(i);
    if(i >= size || left == -1) return;
    else if(right != -1) max = (data[left] > data[right]) ? left : right;
    else max = left;

    if(data[i] < data[max]){
        std::swap(data[i], data[max]);
        heapify(max, size);
    }
}


template<typename T>
void binaryHeapMax<T>::build(){ 
    for(int i = start(); i >= 0; --i){
        heapify(i, size_);
    }
}

template<typename T>
int binaryHeapMax<T>::getLeft(int i) const{ 
    int index = i * 2 + 1;
    if(index >= size_) return -1;
    else return index; 
}

template<typename T>
int binaryHeapMax<T>::getRight(int i) const{ 
    int index = i * 2 + 2;
    if(index >= size_) return -1;
    else return index; 
}

template<typename T>
int binaryHeapMax<T>::getParent(int i) const{ 
    if(i == 0) return -1;
    int index = (i - 1) / 2;
    return index;
}

template<typename T>
void binaryHeapMax<T>::display() const {
    if(empty()){
        std::cout << "Heap empty" << std::endl;
        return;
    }

    int levels = std::floor(std::log2(size_)) + 1;

    int index = 0;
    for(int level = 0; level < levels; ++level){
        int elementsInLevel = std::pow(2, level);
        int spaces = std::pow(2, levels - level) - 1;

        for(int s = 0; s < spaces; ++s){
            std::cout << " ";
        }

        for(int i = 0; i < elementsInLevel && index < size_; ++i, ++index){
            std::cout << data[index];

            for(int s = 0; s < 2 * spaces - 1 && i < elementsInLevel -1; ++s){
                std::cout << " ";
            }
        }

        std::cout << std::endl;
    }
}

#endif