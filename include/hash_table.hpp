#include <iostream>
#pragma once




template <typename Value>
struct Node {
    size_t key;
    Value value;
    Node* next;

    Node(size_t k, const Value& v) : key(k), value(v), next(nullptr) {}
};

template <typename Value>
class HashTableIterator {
public:
    HashTableIterator(Node<Value>* node = nullptr, size_t bucketIndex = 0, size_t tableSize = 0, Node<Value>** tablePtr = nullptr) :
        currentNode(node), currentBucket(bucketIndex), tableSize(tableSize), table(tablePtr) {}

    HashTableIterator(const HashTableIterator& other) = default;

    ~HashTableIterator() = default;

    bool operator==(const HashTableIterator& other) const {
        return currentNode == other.currentNode && table == other.table;
    }

    bool operator!=(const HashTableIterator& other) const {
        return !(*this == other);
    }

    HashTableIterator& operator++() {
        if (currentNode) {
            currentNode = currentNode->next;
            if (!currentNode) {
                ++currentBucket;
                while (currentBucket < tableSize && (table[currentBucket] == nullptr)) {
                    ++currentBucket;
                }
                if (currentBucket < tableSize) {
                    currentNode = table[currentBucket];
                }
            }
        }
        return *this;
    }

    HashTableIterator operator++(int) {
        HashTableIterator temp = *this;
        ++(*this);
        return temp;
    }

    Value operator*() const {
        return currentNode->value;
    }


private:
    Node<Value>* currentNode;
    size_t currentBucket;
    size_t tableSize;
    Node<Value>** table;
};

template <typename Value>
class HashTable {
private:
    Node<Value>** table; 
    size_t size; 
    size_t count; 

    
    size_t hash(size_t key) const {
        return key % size;
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

    void destroy() {
        if (table == nullptr) {
            return;
        }
        for (size_t i = 0; i < size; ++i) {
            Node<Value>* current = table[i];
            while (current != nullptr) {
                Node<Value>* next = current->next;
                delete current;
                current = next;
            }
        }
        delete[] table;
        table = nullptr;
    }
public:
    using value_type = Value;
    using iterator = HashTableIterator<Value>;

    iterator begin() {
        size_t i = 0;
        while (i < size && table[i] == nullptr) {
            ++i;
        }

        if (i==size)
        {
            return iterator(nullptr, i, size, table);
        }
        return iterator(table[i], i, size, table);
    }
    iterator end() {
        return iterator(nullptr, size, size, table);
    }

    HashTable() : size(8), count(0) {
        table = new Node<Value>*[size];
        for (size_t i = 0; i < size; ++i) {
            table[i] = nullptr;
        }
    }

    HashTable(size_t capacity) : size(capacity), count(0) {
        table = new Node<Value>* [size];
        for (size_t i = 0; i < size; ++i) {
            table[i] = nullptr;
        }
    }

    HashTable(const HashTable<Value>& other) {
        count = 0;
        size = other.size;
        table = new Node<Value>*[size];
        for (size_t i = 0; i < size; ++i) {
            table[i] = nullptr;
        }

        for (size_t i = 0; i < other.size; ++i) {
            Node<Value>* current = other.table[i];
            while (current != nullptr) {
                Node<Value>* next = current->next;
                if (find(current->key) == nullptr) {
                    insert(current->key, current->value);
                }
                current = next;
            }
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
                if (other.find(current->key) == nullptr) {
                    result.insert(current->key, current->value);
                }
                current = current->next;
            }
        }

        for (size_t i = 0; i < size; ++i) {
            Node<Value>* current = other.table[i];
            while (current) {
                if (find(current->key) == nullptr) {
                    result.insert(current->key, current->value);
                }
                current = current->next;
            }
        }
        return result;
    }

    Value operator[](const size_t key) {
        Node<Value>* el = find(key);
        if (el == nullptr) {
            return -1;
        }
        return (el->value);
    }

    size_t power() const {
        return count;
    }
    
    bool operator ==(const HashTable<Value>& other) const {
       
        if (count != other.count) {
            return false;
        }
        for (size_t i = 0; i < size; ++i) {
            Node<Value>* current = table[i];
            while (current) {
                
                if (other.find(current->key) == nullptr) {
                    return false;
                }
                current = current->next;
            }
        }
        return true;
    }

    bool operator != (const HashTable<Value>& other) const {
        return !(*this == other);
    }


    HashTable<Value>& operator=(const HashTable<Value>& other) {
        if (table == other.table) {
            return *this;
        }
        destroy();
        size = other.size;
        table = new Node<Value>*[size];
        for (size_t i = 0; i < size; i++) {
            table[i] = nullptr;
        }
        count = 0;
        for (size_t i = 0; i < size; ++i) {
            Node<Value>* current = other.table[i];
            while (current != nullptr) {
                insert(current->key, current->value);
                current = current->next;
            }
        }


        return *this;
    }

    void addingFirstMissingOne() {
        size_t firstMissingOneKey = 0;
        for (size_t i = 0; i < size; ++i) {
            Node<Value>* current = table[i];
            while (current) {
                if (find(firstMissingOneKey) == nullptr) {

                    insert(firstMissingOneKey, Value(firstMissingOneKey));
                    return;
                }
                firstMissingOneKey++;
                current = current->next;
            }
        }

        insert(firstMissingOneKey, Value(firstMissingOneKey));
    }

    friend std::ostream& operator<<(std::ostream& os, const HashTable<Value>& ht) {
        os << "[";
        bool flagBracket = false;
        for (size_t i = 0; i < ht.size; ++i) {
            Node<Value>* current = ht.table[i];
            while (current != nullptr) {
                os << (flagBracket ? " " : "") << current->value;
                current = current->next;
                flagBracket = true;
            }
        }
        os << "]" << std::endl;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, HashTable<Value>& ht) {
        char c;
        is >> c; 
        if (c != '[') {
            is.unget(); 
            return is; 
        }
        char next_char;
        while (is >> next_char) {
            if (next_char == ']') {
                break;
            }
            else {
                is.unget(); 
                Value value;
                is >> value; 
                if (is.fail()) {
                    std::cerr << "Error: Invalid input." << std::endl;
                    return is;
                }
                ht.insert(value, value);
            }
        }
        return is;
    }
};
