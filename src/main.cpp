#include "hash_table.hpp"

int main() {
    setlocale(LC_ALL, "Russian");

    HashTable<int> ht(3);

    ht.insert(0, 0);
    ht.insert(1, 1);
    ht.insert(2, 2);

    HashTable<int> ht1(4);
    ht1.insert(1, 1);
    ht1.insert(2, 2);
    ht1.insert(6, 6);
    ht1.insert(5, 5);

    {
        std::cout << "Baseline HashTable\n";
        std::cout << "First HashTable: \n";
        ht.print();
        std::cout << "Second HashTable: \n";
        ht1.print();
    }

    {
        std::cout << "----------------------------\n";
        std::cout << "UNION CASE(ht and ht1): \n";
        auto result = ht.unionWith(ht1);
        result.print();
        std::cout << result;
    }

    {
        std::cout << "----------------------------\n";
        std::cout << "INTERSECTION CASE(ht and ht1): \n";
        auto result = ht.intersectionWith(ht1);
        result.print();
        std::cout << result;
    }

    {
        std::cout << "----------------------------\n";
        std::cout << "DIFFERENCE CASE(ht and ht1): \n";
        auto result = ht.differenceWith(ht1);
        result.print();
        std::cout << result;
    }

    {
        std::cout << "----------------------------\n";
        std::cout << "SYMETRIC DIFFERENCE CASE(ht and ht1): \n";
        auto result = ht.differenceWith(ht1);
        result.print();
        std::cout << result;
    }

    {
        std::cout << "----------------------------\n";
        std::cout << "CHECKING [](valid index in ht) CASE: \n";
        constexpr int findValidIndex = 0;
        auto* value = ht[findValidIndex];
        if (value != nullptr) {
            std::cout << "key and values found\n";
        }
        else {
            std::cout << "ERROR\n";
        }
    }

    {
        std::cout << "----------------------------\n";
        std::cout << "CHECKING [](invalid index in ht) CASE: \n";
        constexpr int findInvalidIndex = 0;
        auto* value = ht[3];
        if (value == nullptr) {
            std::cout << "key and values not found\n";
        }
        else {
            std::cout << "ERROR\n";
        }
    }

    {
        std::cout << "----------------------------\n";
        std::cout << "CAPACITY(ht) CASE: \n";
        std::cout << "Capacity ht: " << ht.capacity() << "\n";
    }

    {
        std::cout << "----------------------------\n";
        std::cout << "COMPARE(ht and ht1) CASE: \n";
        std::cout << "Compare ht and ht1: " << std::boolalpha << ht.compare(ht1) << "\n";
    }

    {
        std::cout << "----------------------------\n";
        std::cout << "INSERT CASE: \n";
        HashTable<int> result(2);
        std::cout << "Before Insert\n";
        result.print();
        result.insert(0, 0);
        result.insert(1, 1);
        result.insert(2, 2);
        std::cout << "After Insert: \n";
        result.print();
    }

    {
        std::cout << "----------------------------\n";
        std::cout << "ADDING FIRST MISSING ONE CASE: \n";
        HashTable<int> result(2);
        result.insert(0, 0);
        result.insert(1, 1);
        std::cout << "Before adding: \n";
        result.print();
        result.addingFirstMissingOne();
        std::cout << "After adding: \n";
        result.print();
    }

    {
        std::cout << "----------------------------\n";
        std::cout << "CHECK << OPERATOR CASE:\n";
        std::cout << ht;
    }

    /*HashTable<int> ht8(3);
    std::cin >> ht8;
    ht8.print();*/
    std::cout << (*((--ht.end())))->value << "\n";
    //std::cout << *(++ht1.begin()) << "\n";
    /*std::cout << (ht1.begin() == ht1.begin()) << "\n";
    std::cout << (ht1.begin() != ht1.begin()) << "\n";
    std::cout << (ht1.begin() == ht1.end()) << "\n";
    std::cout << (ht1.begin() != ht1.end()) << "\n";*/
    //std::cout << *ht1.end();

    return 0;
}