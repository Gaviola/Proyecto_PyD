//
// Created by andres on 5/21/24.
//

#ifndef PROYECTO_PYD_UTILS_H
#define PROYECTO_PYD_UTILS_H

#include <vector>
#include <iostream>

using namespace std;

void printVector(vector<int> list);
void mergeSort(vector<int>&);
vector<int> merge(std::vector<int>& izquierda, std::vector<int>& derecha);
bool checkOrder(vector<int>& list);
#endif //PROYECTO_PYD_UTILS_H
