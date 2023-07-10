/*Erick Zambrano*/

#include <iostream>
#include <mpi.h>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


static int rank, num_procesos, bloque;

static std::vector<int> vector = {8, 23, 19, 67, 45, 35, 1, 24, 13, 30, 3, 5,4,7,2,5,9,0,45,332,56,7,88,2,3,4};

std::vector<int> ordenar(std::vector<int> vector) {
    std::sort(vector.begin(), vector.end());
    return vector;
}

std::vector<int> mezclar(std::vector<int> vector1, std::vector<int> vector2) {
    std::vector<int> mezcla(vector1.size() + vector2.size());

    int i = 0, j = 0, k = 0;

    while (i < vector1.size() && j < vector2.size()) {
        if (vector1[i] <= vector2[j]) {
            mezcla[k++] = vector1[i++];
        } else {
            mezcla[k++] = vector2[j++];
        }
    }

    while (i < vector1.size()) {
        mezcla[k++] = vector1[i++];
    }

    while (j < vector2.size()) {
        mezcla[k++] = vector2[j++];
    }

    return mezcla;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    // Obtener el rank y el número de procesos
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procesos);

    if (rank == 0) {
        //vector = read_file();
        bloque = vector.size() /num_procesos;
        for (int nRank = 1; nRank <num_procesos; nRank++) {
            MPI_Send(&vector[(nRank-1) * bloque], bloque, MPI_INT, nRank, 0, MPI_COMM_WORLD);
        }

        std::vector<int> vec0(vector.begin() + bloque*(num_procesos-1), vector.end());
        std::vector<int> vecFinal = ordenar(vec0);

        for (int nRank = 1; nRank <num_procesos; nRank++) {
            std::vector<int> vecTemp(bloque);
            MPI_Recv(vecTemp.data(), bloque, MPI_INT, nRank, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            vecFinal = mezclar(vecTemp, vecFinal);
        }

        for (int i = 0; i < vecFinal.size(); ++i) {
            std::printf("{%d}, ", vecFinal[i]);
        }
        std::cout << std::endl;
    } else {
        bloque = vector.size() /num_procesos;
        std::vector<int> vectorTmp(bloque);
        MPI_Recv(vectorTmp.data(), bloque, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        std::vector<int> vectoResp = ordenar(vectorTmp);
        MPI_Send(vectoResp.data(), bloque, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}