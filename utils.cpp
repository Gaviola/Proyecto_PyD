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

bool checkOrder(vector<int>& list) {
    for (int i = 0; i < list.size() - 1; i++) {
        if (list[i] > list[i + 1]) {
            return false;
        }
    }
    return true;
}