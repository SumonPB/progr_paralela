#include <iostream>
#include <fmt/core.h>
#include <mpi.h>
#include <vector>
#define MATRIX_DIH 25

#include <vector>
#include <iostream>

void imprimir_matriz(const std::vector<double>& A_local,
                     int rows,
                     int matrix_dim) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < matrix_dim; j++) {
            std::cout << A_local[i * matrix_dim + j] << " ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char **argv)
{

    MPI_Init(&argc, &argv);
    int nprocs, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // numero de filas para cada RANK (proceso)

    if (rank == 0)
    {
        std::vector<double> A(MATRIX_DIH * MATRIX_DIH);
        std::vector<double> B(MATRIX_DIH);
        std::vector<double> X(MATRIX_DIH);
        // inicializar la matriz A y el vector b
        for (int i = 0; i < MATRIX_DIH; i++)
        {
            for (int j = 0; j < MATRIX_DIH; j++)
            {
                int index = i * MATRIX_DIH + j;
                A[index] = i;
            }
        }
        for (int i = 0; i < MATRIX_DIH; i++)
        {
            B[i] = 1;
        }

        // numero de filas y columnas por procesos
        int rows_per_rank = std::ceil(MATRIX_DIH * 1.0 / nprocs);
        int padding = rows_per_rank * nprocs - MATRIX_DIH;
        fmt::print("MATRIX_DIM{}, nprocs:{}, rows_per_rank:{},padding :{}\n", MATRIX_DIH, nprocs, rows_per_rank, padding);
        // enviar dimensiones y datos
        for (int i = 0; i < nprocs; i++)
        {
            int fila = rows_per_rank;
            if (nprocs - 1 == i)
            {
                fila = rows_per_rank - padding;
            }

            // enviar dimensiones
            std::vector<int> data = {MATRIX_DIH, fila};
            MPI_Send(
                data.data(),   // buffer de datos
                2,             // data.size(),   // cuanto
                MPI_INT,       // Tipo de dato
                i,             // Rank de destino
                0,             // TAG
                MPI_COMM_WORLD // Grupo
            );

            const double *Buffer = A.data();
            // enviar datos al rank i
            MPI_Send(
                &Buffer[i * rows_per_rank * MATRIX_DIH], // buffer de datos
                fila * MATRIX_DIH,                     // data.size(),   // cuanto
                MPI_DOUBLE,                            // Tipo de dato
                i,                                     // Rank de destino
                0,                                     // TAG
                MPI_COMM_WORLD                         // Grupo
            );
        }
        fmt::print("RANK_{}, {} x {}", rank, rows_per_rank, MATRIX_DIH);
    }
    else
    {
        std::vector<int> data_rec(2);
        MPI_Recv(
            data_rec.data(),
            2,
            MPI_INT,
            0,
            0,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
        int matrix_dim = data_rec[0];
        int rows = data_rec[1];
        fmt::print("RANK_{}, {} x {}", rank, matrix_dim, rows);

        std::vector<double> A_local(rows * matrix_dim);
        MPI_Recv(
            A_local.data(),
            rows * matrix_dim,
            MPI_DOUBLE,
            0,
            0,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);

          if (rank==3)
          {
            imprimir_matriz(A_local,rows,matrix_dim);
          }
            
    }

    MPI_Finalize();
    return 0;
}
