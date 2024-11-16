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

    auto ht2 = ht.unionWith(ht1);
    auto ht3 = ht.intersectionWith(ht1);
    auto ht4 = ht.differenceWith(ht1);
    auto ht5 = ht.symetricDifferenceWith(ht1);

    ht.print();
    ht1.print();
    ht2.print();
    ht3.print();
    ht4.print();
    ht5.print();

    auto* ind = ht[0];
    if (ind) {
        std::cout << "FIND []\n";
    }
    auto* ind1 = ht[3];
    if (ind1 == nullptr) {
        std::cout << "NOT FIND []\n";
    }

    std::cout << ht2.capacity() << "\n";

    std::cout << ht.compare(ht1) << "\n";

    HashTable<int> ht6(2);

    ht6.insert(0, 0);
    ht6.insert(1, 1);
    ht6.insert(2, 2);
    std::cout << ht.compare(ht6) << "\n";





    HashTable<int> ht7(2);
    ht7.insert(0, 0);
    ht7.insert(1, 1);
    ht7.addingFirstMissingOne();
    ht7.print();

    return 0;
}