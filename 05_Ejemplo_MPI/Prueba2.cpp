#include<mpi.h>
#include<vector>
#include<cmath>
#include<iostream>

int VSIZE=21;
int main(int argc, char **argv)
{
    std::vector<int> vec (VSIZE,0);
    for (size_t i = 0; i < VSIZE; i++)
    {
        vec[i] = i;
    }
    
    MPI_Init(&argc,&argv);
    int rack,nprocs;
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rack);
    if (rack==0)
    {
        int padding = std::ceil(VSIZE*1.0/nprocs);


        for (size_t i = 1; i < nprocs; i++)
        {
            int cantidad;
                if (i==nprocs-1)
            {
                cantidad = VSIZE-(padding*(nprocs-1));
            }else{
                cantidad=padding;
            }
                std::vector<int> info {cantidad};
                    MPI_Send(
            info.data(),
            1,
            MPI_INT,
            i,
            0,
            MPI_COMM_WORLD
        );
        MPI_Send(
            vec.data()+(i*padding),
            cantidad,
            MPI_INT,
            i,
            0,
            MPI_COMM_WORLD
        );


        }
        //std::cout<<padding<<std::endl;


        
    }else{
        int padding;
        MPI_Recv(
            &padding,
            1,
            MPI_INT,
            0,
            0,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );
        std::vector<int> vecTMP(padding);
        MPI_Recv(
            vecTMP.data(),
            padding,
            MPI_INT,
            0,
            0,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );

        //std::cout<<padding<<std::endl;
        std::cout<<"--------------"<<std::endl;

            for (size_t i = 0; i < vecTMP.size(); i++)
            {
                 std::cout<<vecTMP[i]<<std::endl;
            }

    }
    


    MPI_Finalize();
    return 0;
}
