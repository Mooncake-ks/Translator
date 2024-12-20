#include <iostream>
#pragma once

template<typename Value>
void qsortRecursive(Value* mas, int size) {
    int i = 0;
    int j = size - 1;

    int mid = mas[size / 2];

    do {
        while (mas[i] < mid) {
            i++;
        }
        while (mas[j] > mid) {
            j--;
        }
        if (i <= j) {
            Value tmp = mas[i];
            mas[i] = mas[j];
            mas[j] = tmp;

            i++;
            j--;
        }
    } while (i <= j);

    if (j > 0) {
        qsortRecursive(mas, j + 1);
    }
    if (i < size) {
        qsortRecursive(&mas[i], size - i);
    }
}


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
                // Move to the next non-empty bucket
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

    Node<Value>* operator*() const {
        return currentNode;
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
    Node<Value>** table; // ptr array to nodes
    size_t size; // size table
    size_t count; // count elements

    // hash function
    size_t hash(size_t key) const {
        return key % size;
    }

public:
    using value_type = Value;
    using iterator = HashTableIterator<Value>;

    iterator begin() {
        size_t i = 0;
        while (i < size && table[i] == nullptr) {
            ++i;
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
        destroy();
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

    bool operator == (const HashTable<Value>& other) const {
        return this->count == other.count;
    }

    bool operator != (const HashTable<Value>& other) const {
        return !(*this == other);
    }

    bool operator > (const HashTable<Value>& other) const {
        return count > other.count ? true : false;
    }

    bool operator < (const HashTable<Value>& other) const {
        return !(*this > other);
    }

    bool operator >= (const HashTable<Value>& other) const {
        return *this > other || *this == other;
    }

    bool operator <= (const HashTable<Value>& other) const {
        return *this < other || *this == other;
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
        Value* vectorizedHashTable = new Value[count];
        int arrInd = 0;
        for (size_t i = 0; i < size; ++i) {
            Node<Value>* current = table[i];
            while (current != nullptr) {
                vectorizedHashTable[arrInd] = current->value;
                ++arrInd;
                current = current->next;
            }
        }

        qsortRecursive(vectorizedHashTable, count);
        Value targetValue = 0;
        for (int i = 0; i < count; ++i) {
            if (vectorizedHashTable[i] != targetValue) {
                insert(targetValue, targetValue);
                delete[] vectorizedHashTable;
                return;
            }
            ++targetValue;
        }
        insert(targetValue, targetValue);
        delete[] vectorizedHashTable;
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
        is >> c; //read the '['
        if (c != '[') {
            is.unget(); // put the character back into the stream if it wasnt '['
            return is; // return the stream to signal failure
        }
        char next_char;
        while (is >> next_char) {
            if (next_char == ']') {
                break;
            }
            else {
                is.unget(); // put it back if it wasnt ']'
                Value value;
                is >> value; //read the value
                if (is.fail()) {
                    std::cerr << "Error: Invalid input." << std::endl;
                    return is;
                }
                ht.insert(value, value);
            }
        }
        return is;
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
};
