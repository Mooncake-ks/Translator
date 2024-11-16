#include <iostream>

template <typename Value>
class HashTable {
private:
    struct Node {
        size_t key;
        Value value;
        Node* next;

        Node(const size_t k, const Value& v) : key(k), value(v), next(nullptr) {}
    };

    Node** table; // ”казатель на массив указателей на узлы
    size_t size; // size table
    size_t count; // count elements

    // hash function
    size_t hash(size_t key) const {
        return key % size;
    }

public:
    HashTable(size_t capacity) : size(capacity), count(0) {
        table = new Node * [size];
        for (size_t i = 0; i < size; ++i) {
            table[i] = nullptr;
        }
    }

    ~HashTable() {
        for (size_t i = 0; i < size; ++i) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* next = current->next;
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
        Node* newNode = new Node(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        count++;
    }

    Value* find(const size_t key) const {
        size_t index = hash(key);
        Node* current = table[index];
        while (current != nullptr) {
            if (current->key == key) {
                return &(current->value);
            }
            current = current->next;
        }
        return nullptr;
    }

    void erase(const size_t key) {
        size_t index = hash(key);
        Node* current = table[index];
        Node* previous = nullptr;
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
            Node* current = table[i];
            while (current) {
                result.insert(current->key, current->value);
                current = current->next;
            }
        }
        for (size_t i = 0; i < other.size; ++i) {
            Node* current = other.table[i];
            while (current) {
                result.insert(current->key, current->value);
                current = current->next;
            }
        }
        return result;
    }

    //ok
    HashTable<Value> intersectionWith(const HashTable<Value>& other) const {
        HashTable<Value> result(size < other.size ? size : other.size);
        for (size_t i = 0; i < result.size; ++i) {
            Node* current = table[i];
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
            Node* current = table[i];
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
            Node* current = table[i];
            while (current) {
                //insert what is in the first set but not in the second
                if (other.find(current->key) == nullptr) {
                    result.insert(current->key, current->value);
                }
                current = current->next;
            }
        }

        for (size_t i = 0; i < size; ++i) {
            Node* current = other.table[i];
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
        return find(key);
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
            Node* current = table[i];
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
            Node* current = table[i];
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
            Node* current = table[i];
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
            Node* current = ht.table[i];
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