#include <mpi.h>
#include <math.h>
#include <fmt/core.h>
#include <vector>
#include <cstdlib> // Para rand() y srand()
#include <ctime>   // Para time()

using namespace std;

int Dimension_Matris = 40;

void GenerarMatris(int Matrix_Dim, vector<int> &matrizGeneral)
{
    srand(time(nullptr));
    for (int i = 0; i < Matrix_Dim; i++)
    {
        int numeroCorto = rand() % 101;
        matrizGeneral[i] = numeroCorto;
    }
}

void SumaFilas(vector<int> &A, vector<int> &filas_suma, int fila, int columna)
{
    for (int i = 0; i < fila; i++)
    {
        int sumaTMP = 0;
        for (int j = 0; j < columna; j++)
        {
            sumaTMP += A[i * columna + j];
        }
        filas_suma[i] = sumaTMP;
    }
}

void Promedio(vector<int> &A, int &suma)
{
    for (int i = 0; i < A.size(); i++)
    {
        suma += A[i];
    }
}

void minimo_maximoFuncion(vector<int> &A, vector<int> &min_max)
{
    int minimo = min_max[0];
    int maximo = min_max[1];
    for (int i = 0; i < A.size(); i++)
    {
        if (A[i] < minimo)
        {
            minimo = A[i];
        }

        if (A[i] > maximo)
        {
            maximo = A[i];
        }
    }
    min_max[0] = minimo;
    min_max[1] = maximo;
}

void dobleMatris(vector<int> &A, vector<int> &B)
{
    for (int i = 0; i < A.size(); i++)
    {
        B[i] = A[i] * 2;
    }
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int nprocs, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int total = Dimension_Matris * Dimension_Matris;

    if (rank == 0)
    {
        int rows_per_rank = ceil((Dimension_Matris * 1.0) / nprocs);
        int padded_rows = rows_per_rank * nprocs;
        int nuevoTotal = padded_rows * Dimension_Matris;
        // inicializamos la matriz
        vector<int> matrizA(nuevoTotal, 0);
        GenerarMatris(nuevoTotal, matrizA);
        // Enviar datos correspondientes a los otros ranks
        for (int i = 1; i < nprocs; i++)
        {
            vector<int> info = {rows_per_rank, Dimension_Matris};
            // enviar numero de filas
            MPI_Send(
                info.data(),
                2,
                MPI_INT,
                i,
                0,
                MPI_COMM_WORLD);
            // ENVIAR DATOS
            MPI_Send(
                matrizA.data() + i * rows_per_rank * Dimension_Matris,
                rows_per_rank * Dimension_Matris,
                MPI_INT,
                i,
                0,
                MPI_COMM_WORLD);
        }
        // Procesar parte de rank 0
        vector<int> aTMP(matrizA.begin(), matrizA.begin() + rows_per_rank * Dimension_Matris);

        vector<int> sumaTMP(rows_per_rank, 0);
        vector<int> sumaFilasBuffer(padded_rows, 0);

        SumaFilas(aTMP, sumaTMP, rows_per_rank, Dimension_Matris);
        for (int i = 0; i < sumaTMP.size(); i++)
        {
            sumaFilasBuffer[i] = sumaTMP[i];
        }

        for (int i = 1; i < nprocs; i++)
        {
            MPI_Recv(
                sumaFilasBuffer.data() + i * rows_per_rank,
                rows_per_rank,
                MPI_INT,
                i,
                0,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE);
        }
        for (int i = 0; i < sumaFilasBuffer.size(); i++)
        {
            printf("\n Fila %d -> %d", i, sumaFilasBuffer[i]);
        }
        // procesar promedio
        int sumaTEMP = 0;
        int sumaFinal = 0;
        Promedio(aTMP, sumaTEMP);
        sumaFinal = sumaTEMP;
        for (int i = 1; i < nprocs; i++)
        {
            int sumaParcial = 0;
            MPI_Recv(
                &sumaParcial,
                1,
                MPI_INT,
                i,
                0,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE);
            sumaFinal += sumaParcial;
        }
        int promedio = (sumaFinal * 1.0) / nuevoTotal;
        printf("\n Promedio: %d", promedio);
        // parte del rank 0 para min max
        vector<int> min_max(2, 0);
        int minimo = 0;
        int maximo = 0;
        vector<int> min_maxTMP(2, 0);
        minimo_maximoFuncion(aTMP, min_max);
        minimo = min_max[0];
        maximo = min_max[1];

        for (int i = 1; i < nprocs; i++)
        {
            MPI_Recv(
                min_maxTMP.data(),
                2,
                MPI_INT,
                i,
                0,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE);
            minimo = min_maxTMP[0] < minimo ? min_maxTMP[0] : minimo;
            maximo = min_maxTMP[1] > maximo ? min_maxTMP[1] : maximo;
        }
        printf("\n Minimo: %d", minimo);
        printf("\n Maximo: %d", maximo);
        //parte final doble
        vector<int> bTMP(aTMP.size(),0);
        vector<int> B(nuevoTotal,0);
        dobleMatris(aTMP,bTMP);
        for (int i = 0; i < bTMP.size(); i++)
        {
            B[i] = bTMP[i];
        }
        
        for (int i = 1; i < nprocs; i++)
        {
                MPI_Recv(
                B.data()+i*rows_per_rank*Dimension_Matris,
                rows_per_rank*Dimension_Matris,
                MPI_INT,
                i,
                0,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE);
        }

        
        printf("\n Matriz actual\n");
        for (int i = 0; i < matrizA.size(); i++)
        {
            printf("%d - ",matrizA[i]);
        }
        printf("\n Matriz doble\n");
        for (int i = 0; i < B.size(); i++)
        {
            printf("%d - ",B[i]);
        }
        
        
    }
    // rank diferente de cero
    else
    {
        vector<int> info(2);
        MPI_Recv(
            info.data(),
            2,
            MPI_INT,
            0,
            0,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
        int rowsProcesar = info[0];
        int columnas = info[1];

        vector<int> aTMP(rowsProcesar * columnas, 0);
        MPI_Recv(
            aTMP.data(),
            rowsProcesar * columnas,
            MPI_INT,
            0,
            0,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);

        vector<int> sumasFilasTmp(rowsProcesar, 0);

        SumaFilas(aTMP, sumasFilasTmp, rowsProcesar, columnas);

        // devolver la info al rank 0

        MPI_Send(
            sumasFilasTmp.data(),
            rowsProcesar,
            MPI_INT,
            0,
            0,
            MPI_COMM_WORLD);

        // realizar la suma
        int sumaTMP = 0;
        Promedio(aTMP, sumaTMP);

        MPI_Send(
            &sumaTMP,
            1,
            MPI_INT,
            0,
            0,
            MPI_COMM_WORLD);
        // obtener minimo y maximo
        vector<int> min_max(2, 0);
        minimo_maximoFuncion(aTMP, min_max);

        MPI_Send(
            min_max.data(),
            2,
            MPI_INT,
            0,
            0,
            MPI_COMM_WORLD);

        // doble de la matris
        vector<int> bTMP(aTMP.size(), 0);
        dobleMatris(aTMP, bTMP);

        MPI_Send(
            bTMP.data(),
            rowsProcesar*columnas,
            MPI_INT,
            0,
            0,
            MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
