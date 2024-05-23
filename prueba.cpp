#include <mpi.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include "utils.h"
#include "reader.h"

using namespace std;

void parallelMergeSort(vector<int>&, int, int);

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    vector<int> list;

    if (rank == 0) {
        list = readFile("/home/andres/Documents/LCC/PPD/Proyecto_PyD/randomListGenerator/results/randomList-1000.txt");
        cout << "Cantidad de elementos: " << list.size() << endl;
    }

    auto start = chrono::high_resolution_clock::now();
    parallelMergeSort(list, rank, world_size);

    if (rank == 0) {
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, std::milli> diff = end - start;
        bool isOrdered = checkOrder(list);
        if (isOrdered) {
            cout << "La lista está ordenada" << endl;
        } else {
            cout << "La lista no está ordenada" << endl;
        }

        cout << "Tiempo de ejecución: " << diff.count() << " ms\n";

    }

    MPI_Finalize();
    return 0;
}

void parallelMergeSort(vector<int>& list, int rank, int world_size) {
    int num_elements;
    if (rank == 0) {
        num_elements = list.size();
    }
    // Envía el número total de elementos a todos los nodos
    MPI_Bcast(&num_elements, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int local_num_elements = num_elements / world_size;
    vector<int> local_list(local_num_elements);

    //Divide en partes iguales la lista en base a la cantidad de procesos
    MPI_Scatter(list.data(), local_num_elements, MPI_INT, local_list.data(), local_num_elements, MPI_INT, 0, MPI_COMM_WORLD);

    //Ordena localmente
    mergeSort(local_list);

    //Une las listas en el proceso 0
    vector<int> merged_list(num_elements);
    MPI_Gather(local_list.data(), local_num_elements, MPI_INT, merged_list.data(), local_num_elements, MPI_INT, 0, MPI_COMM_WORLD);

    // Se unen la listas parciales en el proceso 0
    vector<int> final_list;
    if (rank == 0) {
        int current_index = 0;
        for (int i = 0; i < world_size; ++i) {
            vector<int> temp_list(merged_list.begin() + current_index, merged_list.begin() + current_index + local_num_elements);
            if (final_list.empty()) {
                final_list = temp_list;
            } else {
                final_list = merge(final_list, temp_list);
            }
            current_index += local_num_elements;
        }
        list = final_list;
    }
    //printList(final_list);
}