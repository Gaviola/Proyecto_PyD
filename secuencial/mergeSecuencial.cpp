#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>


using namespace std;

void mergeSort(vector<int>&);
vector<int> merge(vector<int>&, std::vector<int>&);
void printList(vector<int>&);
void checkOrder(vector<int>&);

int main() {



    cout << list.size() << endl;
    auto start = chrono::high_resolution_clock::now();
    mergeSort(list);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> diff = end - start;
    checkOrder(list);
    cout << "Tiempo de ejecución: " << diff.count() << " ms\n";
    return 0;
}

// MergeSort


