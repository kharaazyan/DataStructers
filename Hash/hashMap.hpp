#ifndef HASH_MAP
#define HASH_MAP

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <forward_list>

template<typename Key, typename Value>
class hash_map{
    size_t tableSize;
    size_t numElements {0};
    const double loadFactor {0.7};
    std::vector<std::forward_list<std::pair<Key, Value>>> table;
    
    void rehash();
    void update();
    bool isPrime(size_t num);
    size_t getIndex(const Key& key) const;
    size_t nextPrime(size_t current);
    unsigned long hash(const Key& key) const;
public:
    hash_map() noexcept;
    hash_map(size_t size_) noexcept;
    ~hash_map() = default;
    Value& operator[](const Key& key);
    void insert(const Key& key, const Value& value);
    bool find(const Key& key, Value& value) const;
    bool erase(const Key& key);
    size_t size() const { return numElements; }
    bool empty() const { return numElements == 0; }
};

template<typename Key, typename Value>
hash_map<Key, Value>::hash_map() noexcept : tableSize(7), table(tableSize){}

template<typename Key, typename Value>
hash_map<Key, Value>::hash_map(size_t size_) noexcept : tableSize(isPrime(size_) ? size_ : nextPrime(size_)), table(tableSize){}

template<typename Key, typename Value>
void hash_map<Key, Value>::insert(const Key& key, const Value& value){
    size_t index = getIndex(key);
    for(auto &chain : table[index]){
        if(chain.first == key){
            chain.second = value;
            return;
        }
    }
    table[index].push_front(std::make_pair(key, value));
    ++numElements;
    update();
}

template<typename Key, typename Value>
size_t hash_map<Key, Value>::getIndex(const Key& key) const { return static_cast<size_t>(hash(key) % tableSize); } 

template<typename Key, typename Value>
void hash_map<Key, Value>::update(){
    tableSize = table.size();
    double curFactor = static_cast<double>(numElements) / static_cast<double>(tableSize);
    if(curFactor >= loadFactor) rehash();
}

template<typename Key, typename Value>
Value& hash_map<Key, Value>::operator[](const Key& key) {
    size_t index = getIndex(key);
    for(auto & kv : table[index]) {
        if(kv.first == key) {
            return kv.second; 
        }
    }
    table[index].emplace_front(key, Value());
    ++numElements;
    update(); 
    return table[index].front().second;
}

template<typename Key, typename Value>
void hash_map<Key, Value>::rehash(){
    tableSize = nextPrime(tableSize * 2);
    std::vector<std::forward_list<std::pair<Key, Value>>> tmp(tableSize);
    for(auto &chain : table){
        for(auto &i : chain){
            size_t index = getIndex(i.first);
            tmp[index].push_front(i);
        }
    }
    table = std::move(tmp);
}

template<typename Key, typename Value>
bool hash_map<Key, Value>::find(const Key& key, Value& value) const{
    size_t index = getIndex(key);
    for (auto & kv : table[index]) {
        if (kv.first == key) {
            value = kv.second;
            return true;
        }
    }
    return false;
}

template<typename Key, typename Value>
bool hash_map<Key, Value>::erase(const Key& key){
    size_t index = getIndex(key);
    auto &chain = table[index];

    auto prevIt = chain.before_begin();
    for (auto it = chain.begin(); it != chain.end(); ++it) {
        if (it->first == key) {
            chain.erase_after(prevIt);
            --numElements;
            return true;
        }
        ++prevIt;
    }
    return false;
}

template<typename Key, typename Value>
unsigned long hash_map<Key, Value>::hash(const Key& key) const {
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

template<typename Key, typename Value>
bool hash_map<Key, Value>::isPrime(size_t num){
    if (num < 2) return false;
    for(size_t i = 2; i * i <= num; ++i) {
        if(num % i == 0) return false;
    }
    return true;
}

template<typename Key, typename Value>
size_t hash_map<Key, Value>::nextPrime(size_t current){
    while(true) {
        if(isPrime(current)) return current;
        ++current;
    }
}


#endif