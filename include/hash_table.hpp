#include <iostream>
#pragma once


template <typename Value>
struct Node {
    size_t key;
    Value value;
    Node* next;

    Node(const size_t k, const Value& v) : key(k), value(v), next(nullptr) {}
};

template <typename Value>
class HashTable {
private:
    Node<Value>** table; // ”казатель на массив указателей на узлы
    size_t size; // size table
    size_t count; // count elements

    // hash function
    size_t hash(size_t key) const {
        return key % size;
    }

public:
    //using iterator = HashTableIterator<Value>;
    //using const_iterator = HashTableIterator<const Value>;
    //using reverse_iterator = std::reverse_iterator<Iterator<Value>>;
    //using const_reverse_iterator = std::reverse_iterator<HashTableIterator<const Value>>;

    //iterator begin() {
    //    size_t first_el = 0;
    //    
    //    while (find(first_el) == nullptr) {
    //        ++first_el;
    //    }
    //    return iterator(head); 
    //}
    /*iterator end() { return iterator(tail->next); }
    iterator begin() const { return iterator(head); }
    iterator end() const { return iterator(tail->next); }
    const_iterator cbegin() const { return head; }
    const_iterator cend() const { return tail->next; }
    reverse_iterator rbegin() { return std::make_reverse_iterator(tail); }
    reverse_iterator rend() { return std::make_reverse_iterator(head); }
    reverse_iterator rbegin() const { return std::make_reverse_iterator(rbegin()); }
    reverse_iterator rend() const  { return std::make_reverse_iterator(rend()); }
    const_reverse_iterator crbegin() const { return rend(); }
    const_reverse_iterator crend() const { return rbegin(); }*/



    HashTable(size_t capacity) : size(capacity), count(0) {
        table = new Node<Value>* [size];
        for (size_t i = 0; i < size; ++i) {
            table[i] = nullptr;
        }
    }

    ~HashTable() {
        for (size_t i = 0; i < size; ++i) {
            Node<Value>* current = table[i];
            while (current != nullptr) {
                Node<Value>* next = current->next;
                delete current;
                current = next;
            }
        }
        delete[] table;
    }

    void insert(const size_t key, const Value& value) {
        if (find(key) != nullptr) {
            return;
        }
        size_t index = hash(key);
        Node<Value>* newNode = new Node(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        count++;
    }

    Node<Value>* find(const size_t key) const {
        size_t index = hash(key);
        Node<Value>* current = table[index];
        while (current != nullptr) {
            if (current->key == key) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void erase(const size_t key) {
        size_t index = hash(key);
        Node<Value>* current = table[index];
        Node<Value>* previous = nullptr;
        while (current != nullptr) {
            if (current->key == key) {
                if (previous != nullptr) {
                    previous->next = current->next;
                }
                else {
                    table[index] = current->next;
                }
                delete current;
                count--;
                return;
            }
            previous = current;
            current = current->next;
        }
    }

    HashTable<Value> unionWith(const HashTable<Value>& other) const {
        HashTable<Value> result(size > other.size ? size : other.size);
        for (size_t i = 0; i < size; ++i) {
            Node<Value>* current = table[i];
            while (current) {
                result.insert(current->key, current->value);
                current = current->next;
            }
        }
        for (size_t i = 0; i < other.size; ++i) {
            Node<Value>* current = other.table[i];
            while (current) {
                result.insert(current->key, current->value);
                current = current->next;
            }
        }
        return result;
    }

    HashTable<Value> intersectionWith(const HashTable<Value>& other) const {
        HashTable<Value> result(size < other.size ? size : other.size);
        for (size_t i = 0; i < result.size; ++i) {
            Node<Value>* current = table[i];
            while (current) {
                //if the element is not in the second set, it does not fit
                if (other.find(current->key) == nullptr) {
                    current = current->next;
                    continue;
                }
                result.insert(current->key, current->value);
                current = current->next;
            }
        }
        return result;
    }

    HashTable<Value> differenceWith(const HashTable<Value>& other) const {
        HashTable<Value> result(size);
        for (size_t i = 0; i < size; ++i) {
            Node<Value>* current = table[i];
            while (current) {
                //if the element is not in the second set, it is suitable
                if (other.find(current->key) == nullptr) {
                    result.insert(current->key, current->value);
                }
                current = current->next;
            }
        }
        return result;
    }

    HashTable<Value> symetricDifferenceWith(const HashTable<Value>& other) const {
        HashTable<Value> result(size);
        for (size_t i = 0; i < size; ++i) {
            Node<Value>* current = table[i];
            while (current) {
                //insert what is in the first set but not in the second
                if (other.find(current->key) == nullptr) {
                    result.insert(current->key, current->value);
                }
                current = current->next;
            }
        }

        for (size_t i = 0; i < size; ++i) {
            Node<Value>* current = other.table[i];
            while (current) {
                //insert what is in the second set but not in the first
                if (find(current->key) == nullptr) {
                    result.insert(current->key, current->value);
                }
                current = current->next;
            }
        }
        return result;
    }

    Value* operator[](const size_t key) {
        Node<Value>* el = find(key);
        if (el == nullptr) {
            return nullptr;
        }
        return &(el->value);
    }

    size_t capacity() const {
        return count;
    }
    
    bool compare(const HashTable<Value>& other) const {
        //if the number of elements does not match
        if (count != other.count) {
            return false;
        }
        for (size_t i = 0; i < size; ++i) {
            Node<Value>* current = table[i];
            while (current) {
                //if you find an element that is not in the second set
                if (other.find(current->key) == nullptr) {
                    return false;
                }
                current = current->next;
            }
        }
        return true;
    }

    void addingFirstMissingOne() {
        size_t firstMissingOneKey = 0;
        for (size_t i = 0; i < size; ++i) {
            Node<Value>* current = table[i];
            while (current) {
                if (find(firstMissingOneKey) == nullptr) {
                    // (0, 0) (3, 3) -> (0, 0) (1, 1) (3, 3)
                    insert(firstMissingOneKey, Value(firstMissingOneKey));
                    return;
                }
                firstMissingOneKey++;
                current = current->next;
            }
        }
        //Handling the case if all the elements are in a row and you need to insert the next one (0, 0) (1, 1) -> (0, 0) (1, 1) (2, 2)
        insert(firstMissingOneKey, Value(firstMissingOneKey));
    }


    void print() const {
        for (size_t i = 0; i < size; ++i) {
            std::cout << "Index " << i << ": ";
            Node<Value>* current = table[i];
            while (current != nullptr) {
                std::cout << "(" << current->key << ", " << current->value << ") ";
                current = current->next;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& os, const HashTable<Value>& ht) {
        os << "Set: ";
        for (size_t i = 0; i < ht.size; ++i) {
            Node<Value>* current = ht.table[i];
            while (current != nullptr) {
                os << current->value << " ";
                current = current->next;
            }
        }
        os << std::endl;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, HashTable<Value>& ht) {
        int count = 0;
        std::cout << "Input count el: ";
        std::cin >> count;
        Value value;
        while (count) {
            is >> value;
            ht.insert(value, value);
            --count;
        }
        return is;
    }
};

//template<typename Value>
//class HashTableIterator {
//public:
//    using difference_type = std::ptrdiff_t;
//    using value_type = std::remove_cv_t<Value>;
//    using pointer = Type*;
//    using reference = Type&;
//    using iterator_category = std::random_access_iterator_tag;
//    using iterator_concept = std::contiguous_iterator_tag;
//
//    explicit HashTableIterator(Node<Value>* ptr = nullptr) : date(ptr) {}
//
//    HashTableIterator(const HashTableIterator& iterator) : HashTableIterator(iterator.date) {}
//
//    ~HashTableIterator() = default;
//
//    bool operator==(const HashTableIterator& lhs) const {
//        return date == lhs.date;
//    }
//
//    bool operator!=(const HashTableIterator& lhs) const {
//        return date != lhs.date;
//    }
//
//    HashTableIterator operator++() {
//       
//    }
//
//    HashTableIterator operator++(int post) {
//
//    }
//    HashTableIterator operator--() {
//
//    }
//    HashTableIterator operator--(int post) {
//
//    }
//    Value& operator*() {
//
//    }
//private:
//    Node<Value>* date;
//};