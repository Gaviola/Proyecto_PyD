//
// Created by andres on 5/23/24.
//

#include "utils.h"

void printVector(vector<int> list) {
    for (int i = 0; i < list.size(); i++) {
        cout << list[i] << " ";
    }
    cout << endl;
}

vector<int> merge(std::vector<int>& izquierda, std::vector<int>& derecha);

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
    int totalSize = izquierda.size() + derecha.size();
    vector<int> listaOrdenada;
    listaOrdenada.reserve(totalSize); // Reserve space in the vector

    int i = 0, j = 0;
    while (i < izquierda.size() || j < derecha.size()) {
        if (i < izquierda.size() && j < derecha.size()) {
            if (izquierda[i] < derecha[j]) {
                listaOrdenada.emplace_back(std::move(izquierda[i])); // Use std::move to avoid copying
                i++;
            } else {
                listaOrdenada.emplace_back(std::move(derecha[j])); // Use std::move to avoid copying
                j++;
            }
        } else if (i < izquierda.size()) {
            listaOrdenada.emplace_back(std::move(izquierda[i])); // Use std::move to avoid copying
            i++;
        } else if (j < derecha.size()) {
            listaOrdenada.emplace_back(std::move(derecha[j])); // Use std::move to avoid copying
            j++;
        }
    }
    return listaOrdenada;
}

bool checkOrder(vector<int>& list) {
    for (int i = 0; i < list.size() - 1; i++) {
        if (list[i] > list[i + 1]) {
            return false;
        }
    }
    return true;
}