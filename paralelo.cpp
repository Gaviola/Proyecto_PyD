#include <mpi.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <queue>
#include "utils.h"
#include "reader.h"

using namespace std;

vector<double> parallelMergeSort(vector<int>&, int, int);
void sequentialMerge(vector<int>& list, int world_size, int local_num_elements);
void mergeK(vector<int>& list, int worldSize, int numElements);

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size, rank;
    // Inicializar variables de mpi
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    vector<int> list;
    // Obtener path al archivo de entrada
    string path = "randomListGenerator/results/randomList-1000000.txt";

    if (rank == 0) {
        list = readFile(path);
        cout << "Cantidad de elementos: " << list.size() << endl;
    }

    auto start = chrono::high_resolution_clock::now();
    // Ordenar las listas de forma paralela
    // Obtener el tiempo de comunicacion
    vector<double> comunitactionTime = parallelMergeSort(list, rank, world_size);

    chrono::duration<double, std::milli> parallelDiff;
    auto end = chrono::high_resolution_clock::now();
    parallelDiff = end - start;

    if (rank == 0) {
        bool isOrdered = checkOrder(list);
        cout << "Tiempo de ejecución paralelo: " << parallelDiff.count() << " ms\n";
        //Imprimir porcentaje de tiempo de comunicaciones
        cout << "Porcentaje de tiempo de comunicaciones: " << (comunitactionTime[0] / parallelDiff.count()) * 100 << "%" << endl;
        //Imprimir porcentaje de tiempo de trabajo del master
        cout << "Porcentaje de tiempo de trabajo del master: " << (comunitactionTime[1] / parallelDiff.count()) * 100 << "%" << endl;
    }

    cout << "Soy el proceso " << rank << " y mi porcentaje de tiempo de trabajo es: " << (comunitactionTime[2] / parallelDiff.count()) * 100 << "%" << endl;
    MPI_Finalize();


    if (rank == 0) {
        // Luego ejecutamos de manera secuencial para comparar
        list = readFile(path);
        auto sstart = chrono::high_resolution_clock::now();
        mergeSort(list);
        auto send = chrono::high_resolution_clock::now();
        chrono::duration<double, std::milli> sdiff = send - sstart;

        // Verificar que la lista este ordenada
        bool isOrdered = checkOrder(list);
        if (isOrdered) {
            cout << "La lista está ordenada" << endl;
        } else {
            cout << "La lista no está ordenada" << endl;
        }

        // Imprimir los resultados
        cout << "Tiempo de ejecución secuencial: " << sdiff.count() << " ms\n";

        cout << "Speedup: " << sdiff.count() / parallelDiff.count() << endl;
    }

    return 0;
}

vector<double> parallelMergeSort(vector<int>& list, int rank, int world_size) {
    int num_elements;
    double masterTimeDiff = 0;
    vector<int> reamining_elements_list;
    double workDiff = 0;
    if (rank == 0) {
        num_elements = list.size();
    }
    //Medimos tiempo de comunicaciones
    auto startComuniaction = chrono::high_resolution_clock::now();

    // Envía el número total de elementos a todos los nodos
    MPI_Bcast(&num_elements, 1, MPI_INT, 0, MPI_COMM_WORLD);
    //Termina primera comunicación
    auto endComuniaction = chrono::high_resolution_clock::now();
    int local_num_elements = num_elements / world_size;
    int remaining_elements = num_elements % world_size;
    vector<int> local_list(local_num_elements);
    //Medir segunda comunicación
    auto startComuniaction2 = chrono::high_resolution_clock::now();
    //Divide en partes iguales la lista en base a la cantidad de procesos
    MPI_Scatter(list.data(), local_num_elements, MPI_INT, local_list.data(), local_num_elements, MPI_INT, 0, MPI_COMM_WORLD);
    //Termina segunda comunicación
    auto endComuniaction2 = chrono::high_resolution_clock::now();
    //Medir tiempo de cada proceso en ordenar localmente
    auto startWork = chrono::high_resolution_clock::now();
    //Ordena localmente
    mergeSort(local_list);
    //Termina tiempo de trabajo
    auto endWork = chrono::high_resolution_clock::now();
    //Calculo de tiempo de trabajo
    workDiff = chrono::duration<double, std::milli>(endWork - startWork).count();
    //Terecer comunicación
    auto startComuniaction3 = chrono::high_resolution_clock::now();
    //Une las listas en el proceso 0
    MPI_Gather(local_list.data(), local_num_elements, MPI_INT, list.data(), local_num_elements, MPI_INT, 0, MPI_COMM_WORLD);
    //Termina tercera comunicación
    auto endComuniaction3 = chrono::high_resolution_clock::now();
    // Se unen la listas parciales en el proceso 0
    if (rank == 0) {
        //Timepo de trabajo del master
        auto startMasterWOrk = chrono::high_resolution_clock::now();
        //agrego los elementos restantes a la lista de elementos restantes
        for (int i = 0; i < remaining_elements; i++) {
            reamining_elements_list.push_back(list[world_size * local_num_elements + i]);
        }
        //Ordena la lista final
        mergeK(list, world_size, local_num_elements);
        //Verifico si hay elementos restantes
        if (remaining_elements != 0) {
            //Los inserto en la lista de manera ordenada
            for(int valor : reamining_elements_list) {
                //Inserto los elementos restantes en la lista final de manera ordenada
                auto it = lower_bound(list.begin(), list.end(), valor);
                list.insert(it, valor);
            }
        }
        //Termina trabajo del master
        auto endMasterWork = chrono::high_resolution_clock::now();
        //calculo de tiempo de trabajo del master
        masterTimeDiff = chrono::duration<double, std::milli>(endMasterWork - startMasterWOrk).count();
    }
    //Calcular tiempo en milisegundos de las comunicaciones
    chrono::duration<double, std::milli> communicationDiff = endComuniaction - startComuniaction + endComuniaction2 - startComuniaction2 + endComuniaction3 - startComuniaction3;
    vector<double> communicationTimes(3);
    communicationTimes[0] = communicationDiff.count();
    communicationTimes[1] = masterTimeDiff;
    communicationTimes[2] = workDiff;
    return communicationTimes;
}

// Algoritmo eficiente para fusionar n listas ordenadas
void mergeK(vector<int>& list, int worldSize, int numElements) {
    // Array para trackear el indice dentro de cada sublista
    vector<int> current_index(worldSize, 0);

    // Min heap para guardar los elementos de cada sublista
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> minHeap;

    // Inicializamos el heap con el primer elemento de cada lista
    for (int i = 0; i < worldSize; ++i) {
        if (current_index[i] < numElements) {
            minHeap.push({list[i * numElements + current_index[i]], i});
            ++current_index[i];
        }
    }

    // Crear una nueva lista que guarde el resultado anterior
    vector<int> mergedList(worldSize * numElements);
    int index = 0;

    // Mergear las listas ordenadas
    while (!minHeap.empty()) {
        auto minElement = minHeap.top();
        minHeap.pop();

        // Añadir el menor elementos a la lista final
        mergedList[index++] = minElement.first;

        // si hay elementos restantes en la lista actual, añadir el siguiente elemento al heap
        if (current_index[minElement.second] < numElements) {
            minHeap.push({list[minElement.second * numElements + current_index[minElement.second]], minElement.second});
            ++current_index[minElement.second];
        }
    }

    // Copiar la lista ordenada a la lista original
    list = mergedList;
}


// Merge sort secuencial
void sequentialMerge(vector<int>& list, int world_size, int local_num_elements) {
    vector<int> final_list;
    int current_index = 0;
    for (int i = 0; i < world_size; ++i) {
        vector<int> temp_list(list.begin() + current_index, list.begin() + current_index + local_num_elements);
        if (final_list.empty()) {
            final_list = temp_list;
            final_list = merge(final_list, temp_list);
        } else {
            current_index += local_num_elements;
        }
    }

    list = final_list;
}