#include<iostream>
#include<mpi.h>

int main(int argc, char** argv){
    //inicializar MPI
    MPI_Init(&argc, &argv);

    //obtener el rank y numero de procesos
    int rank, nprocess;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //rank
    MPI_Comm_size(MPI_COMM_WORLD, &nprocess); //procesos

    if(rank==0){
        std::printf("MPI group size %d\n", nprocess);
    }else{
        std::printf("Soy el rank %d\n", rank);
    }

    //finaliza
    MPI_Finalize();
    
    return 0;
}