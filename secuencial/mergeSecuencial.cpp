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
    ifstream file("/home/fgaviola/CLionProjects/Proyecto_PyD/randomListGenerator/results/randomList-1000000.txt");
    string linea;
    vector<int> list;

    int num;
    if (file.is_open()) {
        while (getline(file, linea)) {
            num = stoi(linea);
            list.push_back(num);
        }
    } else {
        cout << "No se pudo abrir el archivo" << endl;
    }
    file.close();


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
void mergeSort(vector<int>& list) {
    if (list.size() == 1) {
        return;
    } else {
        int medio = list.size() / 2;
        std::vector<int> izquierda(list.begin(), list.begin() + medio);
        std::vector<int> derecha(list.begin() + medio, list.end());
        mergeSort(izquierda);
        mergeSort(derecha);
        vector<int> listOrdenada = merge(izquierda, derecha);
        list = listOrdenada;
    }
}

// Merge
vector<int> merge(std::vector<int>& izquierda, std::vector<int>& derecha) {
    vector<int> listaOrdenada;
    int i = 0, j = 0;
    while (i < izquierda.size() || j < derecha.size()) {
        if (i < izquierda.size() && j < derecha.size()) {
            if (izquierda[i] < derecha[j]) {
                listaOrdenada.push_back(izquierda[i]);
                i++;
            } else {
                listaOrdenada.push_back(derecha[j]);
                j++;
            }
        } else if (i < izquierda.size()) {
            listaOrdenada.push_back(izquierda[i]);
            i++;
        } else if (j < derecha.size()) {
            listaOrdenada.push_back(derecha[j]);
            j++;
        }
    }
    return listaOrdenada;
}

void printList(vector<int>& list) {
    for (int i : list) {
        cout << i << " ";
    }
    cout << endl;
}

void checkOrder(vector<int>& list) {
    for (int i = 0; i < list.size() - 1; i++) {
        if (list[i] > list[i + 1]) {
            cout << "La lista no está ordenada en la posicion" << i << endl;
            return;
        }
    }
    cout << "La lista está ordenada" << endl;
}

