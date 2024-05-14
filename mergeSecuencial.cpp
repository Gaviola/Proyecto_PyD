#include <vector>
#include <iostream>
#include <fstream>

using std::vector, std::printf, std::cout, std::endl;

void mergeSort(vector<int>&);
vector<int> merge(std::vector<int>&, std::vector<int>&);
void printList(vector<int>&);

int main() {
    std:: ifstream file("/home/fgaviola/CLionProjects/Proyecto_PyD/coso.txt");
    std::string linea;
    vector<int> list;

    int num;
    if (file.is_open()) {
        while (getline(file, linea)) {
            num = std::stoi(linea);
            list.push_back(num);
        }
    } else {
        cout << "No se pudo abrir el archivo" << endl;
    }
    cout << "Lista original: " << endl;
    for (int i : list) {
        cout << i << " ";
    }
    cout << endl;
    mergeSort(list);
    cout << "Lista ordenada: " << endl;
    for (int i : list) {
        cout << i << " ";
    }
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
        printList(izquierda);
        printList(derecha);
        mergeSort(izquierda);
        mergeSort(derecha);
        printList(izquierda);
        printList(derecha);
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

