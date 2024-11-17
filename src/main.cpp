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

    {
        /*
        * Indx 0: (3, 3)
        * Indx 1: (7, 7) (1, 1)
        * Indx 2: (2, 2)
        */
        std::cout << "----------------------------\n";
        std::cout << "CHECK ++ OPERATOR CASE:\n";
        HashTable<int> ht8(3);
        ht8.insert(1, 1);
        ht8.insert(2, 2);
        ht8.insert(3, 3);
        ht8.insert(7, 7);
        ht8.print();

        //value - 3
        auto firstValue = (ht8.begin()).currentNode->value;
        //num bucket - 0
        auto NumBucket = (ht8.begin()).currentBucket;
        std::cout << "\nNode value: " << firstValue << " Num bucket: " << NumBucket << "\n";

        //value - 7
        auto secondValue = (++ht8.begin()).currentNode->value;
        //num bucket - 1
        auto NumBucket1 = (++ht8.begin()).currentBucket;
        std::cout << "\nNode value: " << secondValue << " Num bucket: " << NumBucket1 << "\n";

        //value - 1
        auto thirdValue = (++(++ht8.begin())).currentNode->value;
        //num bucket - 1
        auto NumBucket2 = (++(++ht8.begin())).currentBucket;
        std::cout << "\nNode value: " << thirdValue << " Num bucket: " << NumBucket2 << "\n";

        //value - 2
        auto fourthValue = (++(++(++ht8.begin()))).currentNode->value;
        //num bucket - 2
        auto NumBucket3 = (++(++(++ht8.begin()))).currentBucket;
        std::cout << "\nNode value: " << fourthValue << " Num bucket: " << NumBucket3 << "\n";
    }

    {
        /*
        * Indx 0: (3, 3)
        * Indx 1: (7, 7) (1, 1)
        * Indx 2: (2, 2)
        */
        std::cout << "----------------------------\n";
        std::cout << "CHECK -- OPERATOR CASE:\n";
        HashTable<int> ht8(3);
        ht8.insert(1, 1);
        ht8.insert(2, 2);
        ht8.insert(3, 3);
        ht8.insert(7, 7);
        ht8.print();

        auto value = ht8.begin();
        std::cout << "\nNode value: " << value.currentNode->value << " Num bucket: " << value.currentBucket << "\n";
        ++value;
        --value;
        std::cout << "\nNode value: " << value.currentNode->value << " Num bucket: " << value.currentBucket << "\n";
        (++(++value));
        (--(--value));
        std::cout << "\nNode value: " << value.currentNode->value << " Num bucket: " << value.currentBucket << "\n";
        (++(++(++value)));
        (--(--(--value)));
        std::cout << "\nNode value: " << value.currentNode->value << " Num bucket: " << value.currentBucket << "\n";

    }
    return 0;
}