#ifndef HASH_SET
#define HASH_SET

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <forward_list>

template<typename Key>
class hash_set{
    size_t tableSize;
    size_t numElements {0};
    const double loadFactor {0.7};
    std::vector<std::forward_list<Key>> table;
    
    void rehash();
    void update();
    bool isPrime(size_t num);
    size_t getIndex(const Key& key) const;
    size_t nextPrime(size_t current);
    unsigned long hash(const Key& key) const;
public:
    hash_set() noexcept;
    hash_set(size_t size_) noexcept;
    ~hash_set() = default;
    const Key& operator[](const Key& key);
    void insert(const Key& key);
    bool find(const Key& key) const;
    bool erase(const Key& key);
    size_t size() const { return numElements; }
    bool empty() const { return numElements == 0; }
};

template<typename Key>
hash_set<Key>::hash_set() noexcept : tableSize(7), table(tableSize){}

template<typename Key>
hash_set<Key>::hash_set(size_t size_) noexcept : tableSize(isPrime(size_) ? size_ : nextPrime(size_)), table(tableSize){}

template<typename Key>
void hash_set<Key>::insert(const Key& key){
    size_t index = getIndex(key);
    for(auto &chain : table[index]){
        if(chain == key) return;
    }
    table[index].push_front(key);
    ++numElements;
    update();
}

template<typename Key>
size_t hash_set<Key>::getIndex(const Key& key) const { return static_cast<size_t>(hash(key) % tableSize); } 

template<typename Key>
void hash_set<Key>::update(){
    tableSize = table.size();
    double curFactor = static_cast<double>(numElements) / static_cast<double>(tableSize);
    if(curFactor >= loadFactor) rehash();
}

template<typename Key>
const Key& hash_set<Key>::operator[](const Key& key) {
    size_t index = getIndex(key);
    for(auto & kv : table[index]) {
        if(kv == key) return kv;
    }
    table[index].emplace_front(key);
    ++numElements;
    update(); 
    return table[index].front();
}

template<typename Key>
void hash_set<Key>::rehash(){
    tableSize = nextPrime(tableSize * 2);
    std::vector<std::forward_list<Key>> tmp(tableSize);
    for(auto &chain : table){
        for(auto &i : chain){
            size_t index = getIndex(i);
            tmp[index].push_front(i);
        }
    }
    table = std::move(tmp);
}

template<typename Key>
bool hash_set<Key>::find(const Key& key) const{
    size_t index = getIndex(key);
    for (auto & kv : table[index]) {
        if (kv == key) return true;
    }
    return false;
}

template<typename Key>
bool hash_set<Key>::erase(const Key& key){
    size_t index = getIndex(key);
    auto &chain = table[index];

    auto prevIt = chain.before_begin();
    for (auto it = chain.begin(); it != chain.end(); ++it) {
        if (*it == key) {
            chain.erase_after(prevIt);
            --numElements;
            return true;
        }
        ++prevIt;
    }
    return false;
}

template<typename Key>
unsigned long hash_set<Key>::hash(const Key& key) const {
    if constexpr (std::is_same_v<Key, std::string>) {
        const unsigned long FNV_prime = 16777619;
        unsigned long hash = 2166136261;
        for(auto c : key) {
            hash ^= c;
            hash *= FNV_prime;
        }
        return hash;
    }
    else if constexpr (std::is_same_v<Key, const char*>) {
        const unsigned long FNV_prime = 16777619;
        unsigned long hash = 2166136261;
        for(int i = 0; key[i] != '\0'; ++i) {
            hash ^= key[i];
            hash *= FNV_prime;
        }
        return hash;
    }
    else {
        size_t hash = static_cast<size_t>(key);
        hash *= 0x9e3779b97f4a7c15; 
        hash ^= (hash >> 30);
        hash *= 0xbf58476d1ce4e5b9;
        hash ^= (hash >> 27);
        hash *= 0x94d049bb133111eb;
        hash ^= (hash >> 31);
        return hash;
    }
}

template<typename Key>
bool hash_set<Key>::isPrime(size_t num){
    if (num < 2) return false;
    for(size_t i = 2; i * i <= num; ++i) {
        if(num % i == 0) return false;
    }
    return true;
}

template<typename Key>
size_t hash_set<Key>::nextPrime(size_t current){
    while(true) {
        if(isPrime(current)) return current;
        ++current;
    }
}


#endif