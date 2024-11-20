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
class HashTableIterator {
public:
    HashTableIterator(Node<Value>* node = nullptr, size_t bucketIndex = 0, size_t tableSize = 0, Node<Value>** tablePtr = nullptr) :
        currentNode(node), currentBucket(bucketIndex), tableSize(tableSize), table(tablePtr) {}

    HashTableIterator(const HashTableIterator& other) :
        currentNode(other.currentNode), currentBucket(other.currentBucket), tableSize(other.tableSize), table(other.table) {}

    ~HashTableIterator() = default;

    bool operator==(const HashTableIterator& other) const {
        return currentNode == other.currentNode;
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

    HashTableIterator& operator--() {
        //if (currentNode == nullptr) {
        //    --currentBucket;
        //    while (currentBucket > 0 && table[currentBucket] == nullptr) {
        //        --currentBucket;
        //    }
        //    if (currentBucket >= 0) {
        //        currentNode = table[currentBucket];
        //        while (currentNode->next) {
        //            currentNode = currentNode->next;
        //        }
        //    }
        //}
        //else {
        //    if (currentNode->next == nullptr) {
        //        --currentBucket;
        //        while (currentBucket >= 0 && table[currentBucket] == nullptr) {
        //            --currentBucket;
        //        }
        //        if (currentBucket >= 0) {
        //            currentNode = table[currentBucket];
        //            while (currentNode->next) {
        //                currentNode = currentNode->next;
        //            }
        //        }
        //        else {
        //            currentNode = nullptr;
        //        }
        //    }
        //    else {
        //        // find last element in the chain
        //        Node<Value>* last = currentNode;
        //        while (last->next != nullptr) {
        //            last = last->next;
        //        }
        //        currentNode = last;
        //    }
        //}
        //return *this;
        if (currentNode == nullptr) {
            --currentBucket;
            while (currentBucket > 0 && table[currentBucket] == nullptr) {
                --currentBucket;
            }
            if (currentBucket >= 0) {
                currentNode = table[currentBucket];
                while (currentNode->next) {
                    currentNode = currentNode->next;
                }
            }
        }
        else {
            
            auto tmpNode = table[currentBucket];
            //if there are several nodes in the bucket or we are on the very first node in the bucket or empty bucket
            if (table[currentBucket] != currentNode) {
                auto* tmpNode = table[currentBucket];
                while (tmpNode->next != currentNode) {
                    ++tmpNode;
                }
                currentNode = tmpNode;
                return *this;
            }
            else {
                //go to the previous bucket, but not to the nullptr bucket
                --currentBucket;
                while (currentBucket > 0 && table[currentBucket] == nullptr) {
                    --currentBucket;
                }
                if (currentBucket < 0) {
                    currentNode = nullptr;
                    return *this;
                }
                auto* tmpNode = table[currentBucket];
                while (tmpNode->next != nullptr) {
                    tmpNode = tmpNode->next;
                }
                currentNode = tmpNode;
                return *this;
            }
        }
        return *this;
    }

    HashTableIterator operator--(int) {
        HashTableIterator temp = *this;
        --(*this);
        return temp;
    }

    Node<Value>* operator*() const {
        return currentNode;
    }



    Node<Value>* currentNode;
    size_t currentBucket;
    size_t tableSize;
    Node<Value>** table;

    //friend class HashTable<Value>; //Allow HashTable access to private members
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
        char c;
        is >> c; //read the '['
        if (c != '[') {
            is.unget(); // put the character back into the stream if it wasnt '['
            return is; // return the stream to signal failure
        }
        std::string line;
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
};
