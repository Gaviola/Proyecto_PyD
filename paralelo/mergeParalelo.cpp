#include <mpi.h>
#include <vector>
#include "utils.h"

using namespace std;

void mergeParalelo(vector<int>& list, int rank, int size){
    // Cada nodo intenta dividir la lista en dos partes iguales
    // Le envia una parte al nodo con rank 2n+1 y la otra al nodo 2n+2
    // Si superan el rank, el mismo nodo ordena la lista
    cout << "Soy el proceso: " << rank << endl;
    if (rank != 0) {
        // Recibir la lista de otro proceso
        MPI_Status reciveStatus;
        cout << "Before probe" << endl;
        MPI_Probe((rank - 1) / 2, 0, MPI_COMM_WORLD, &reciveStatus);
        cout << "After probe:" << endl;
        int listSize;
        MPI_Get_count(&reciveStatus, MPI_INT, &listSize);
        list.resize(listSize);
        MPI_Recv(list.data(), listSize, MPI_INT, (rank - 1) / 2, 0, MPI_COMM_WORLD, &reciveStatus);
    }

    if(2*rank+1 < size){
        int mid = list.size() / 2;
        vector<int> work = vector<int>(list.begin(),list.begin() +  mid);
        vector<int> listToSend = vector<int>(list.begin() + mid, list.end());

        // Enviar la lista a ambos nodos hijos
        MPI_Request request;
        MPI_Send(listToSend.data(), listToSend.size(), MPI_INT, 2*rank+1, 0, MPI_COMM_WORLD);
        MPI_Send(work.data(), work.size(), MPI_INT, 2*rank+2, 0, MPI_COMM_WORLD);
        cout << "Soy el proceso: " << rank << " enviando a " << 2*rank+1 << " y " << 2*rank+2 << endl;

        // LLamar a merge paralelo en los hijos
        cout << "Calling mergeParalelo" << endl;
        mergeParalelo(work,2*rank+1,size);
        cout << "Primera parte finalizada" << endl;
        mergeParalelo(list,2*rank+2,size);

        // Esperar los resultados
        MPI_Status status;
        MPI_Probe(2*rank+1, 0, MPI_COMM_WORLD, &status);
        int listSize;
        MPI_Get_count(&status, MPI_INT, &listSize);
        work.resize(listSize);

        MPI_Recv(work.data(), listSize, MPI_INT, 2*rank+1, 0, MPI_COMM_WORLD, &status);

        MPI_Probe(2*rank+2, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &listSize);
        list.resize(listSize);

        MPI_Recv(list.data(), listSize, MPI_INT, 2*rank+2, 0, MPI_COMM_WORLD, &status);

        // Fusionar las listas
        list = merge(work,list);

        // Si no es el primer nodo, enviar la lista al padre
        if(rank != 0){
            MPI_Send(list.data(), list.size(), MPI_INT, (rank - 1) / 2, 0, MPI_COMM_WORLD);
        }

    } else {
        mergeSort(list);
    }
}



int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    vector<int> list;
    int number_of_elements;
    if(rank == 0){
        list = {5,7,23,1,9,10,45,0,3,1,2,3,5,65,1,213,-10,675,666,123};
        number_of_elements = list.size();
        cout << "Soy el proceso: " << rank << " y tengo " << number_of_elements << " elementos" << endl;
    }

    MPI_Bcast(&number_of_elements, 1, MPI_INT, 0, MPI_COMM_WORLD);


    vector<int> sublist;
    int elements_per_proc = number_of_elements / size;

    cout << "Soy el proceso: " << rank << " y tengo " << elements_per_proc << " elementos" << endl;

// Resize the sublist vector to hold the elements it will receive
    sublist.resize(elements_per_proc);
    MPI_Scatter(list.data(), elements_per_proc, MPI_INT, sublist.data(), number_of_elements, MPI_INT, 0, MPI_COMM_WORLD);
    printVector(sublist);
    mergeSort(sublist);
    MPI_Gather(sublist.data(), elements_per_proc, MPI_INT, list.data(), elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
    (rank == 0) ? printVector(list) : void();

    // Este ejemplo usa 2n-1 procesos para un grado de paralelismo de n
    MPI_Finalize();
    return 0;
}
