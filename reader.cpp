//
// Created by andres on 5/23/24.
//


#include "reader.h"
#include <string>
#include <vector>
#include <fstream>

using namespace std;
vector<int> readFile(string path) {
    ifstream file(path);
    string linea;
    vector<int> list;

    int num;
    if (file.is_open()) {
        while (getline(file, linea)) {
            num = stoi(linea);
            list.push_back(num);
        }
    }

    file.close();

    return list;
}