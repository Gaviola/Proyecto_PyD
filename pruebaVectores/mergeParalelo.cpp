#include <mpi.h>
#include <vector>

using namespace std;

void mergeParalelo(vector<int>& list){

}



int main() {
    MPI_Init(nullptr, nullptr);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vector<int> list;
    if(rank == 0){
        list = {5,7,23,1,9,10,45,0,3};
    }
    vector<int> work;

    if(rank == 0){
        int mid = list.size() / 2;
        work = vector<int>(list.begin(),list.begin() +  mid);
        vector<int> listToSend = vector<int>(list.begin() + mid, list.end());
        MPI_Send(listToSend.data(), listToSend.size(),MPI_INT,1,0,MPI_COMM_WORLD);
    } else{
        MPI_Status reciveStatus;
        MPI_Probe(0,0,MPI_COMM_WORLD, &reciveStatus);
        int listSize;
        MPI_Get_count(&reciveStatus,MPI_INT, &listSize);
        work.resize(listSize);
        MPI_Recv(work.data(),listSize,MPI_INT,0,0,MPI_COMM_WORLD,&reciveStatus);

    }

    std::cout << "Soy el proceso: " << rank << endl ;
    std::cout << "Longitud: " << work.size() << endl ;
    for(int i : work){
        cout << "valor:" << i << endl;
    }


    MPI_Finalize();
    return 0;
}
