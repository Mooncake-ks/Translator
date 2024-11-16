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

    Node** table; // Указатель на массив указателей на узлы
    size_t size; // Размер таблицы
    size_t count; // Количество элементов

    // Хэш-функция
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

    // Вставка пары ключ-значение
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

    // Поиск значения по ключу
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

    // Удаление пары по ключу
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

    //ok
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
    //ok
    HashTable<Value> differenceWith(const HashTable<Value>& other) const {
        HashTable<Value> result(size);
        for (size_t i = 0; i < size; ++i) {
            Node* current = table[i];
            while (current) {
                if (other.find(current->key) == nullptr) {
                    result.insert(current->key, current->value);
                }
                current = current->next;
            }
        }
        return result;
    }
    //ok
    HashTable<Value> symetricDifferenceWith(const HashTable<Value>& other) const {
        HashTable<Value> result(size);
        for (size_t i = 0; i < size; ++i) {
            Node* current = table[i];
            while (current) {
                if (other.find(current->key) == nullptr) {
                    result.insert(current->key, current->value);
                }
                current = current->next;
            }
        }
        for (size_t i = 0; i < size; ++i) {
            Node* current = other.table[i];
            while (current) {
                if (find(current->key) == nullptr) {
                    result.insert(current->key, current->value);
                }
                current = current->next;
            }
        }
        return result;
    }
    //ok
    Value* operator[](const size_t key) {
        return find(key);
    }

    //ok
    size_t capacity() const {
        return count;
    }
    
    //ok
    bool compare(const HashTable<Value>& other) const {
        if (count != other.count) {
            return false;
        }
        for (size_t i = 0; i < size; ++i) {
            Node* current = table[i];
            while (current) {
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
        //Обработка случая, если все элементы идут подряд и нужно вставить следующий (0, 0) (1, 1) -> (0, 0) (1, 1) (2, 2)
        insert(firstMissingOneKey, Value(firstMissingOneKey));
    }


    // Вывод таблицы
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
};