#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <omp.h>
#define SEED 35791246

int main(int argc, char **argv)
{
    // --- DON'T TOUCH ---
    MPI_Init(&argc, &argv);
    double start_time = MPI_Wtime();
    double pi_result;
    long long int tosses = atoi(argv[1]);
    int world_rank, world_size;
    // ---
    long long int total_count, count=0;
    total_count = 0;
    // TODO: init MPI
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD,&world_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
    long long int niter = tosses/world_size;
    unsigned int rand_state = time(NULL);
    //int recieved[world_size];
    //long recvniter[world_size];
    srand(SEED+world_rank); 

    if (world_rank > 0)
    {
        // TODO: handle workers
        unsigned int rand_state = SEED*world_rank;
        for(int i=0 ; i<niter ; ++i){
            double x = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
            double y = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
            double dist = x*x + y*y;
            if(dist<=1){
                count+=1;
            }
        }
        //printf("count %d of world_rank %d\n", count, world_rank);
        MPI_Send(&count,1,MPI_UNSIGNED_LONG,0,0,MPI_COMM_WORLD);
        //MPI_Send(&niter,1,MPI_LONG,0,2, MPI_COMM_WORLD);

        
        
    }
    else if (world_rank == 0)
    {
        unsigned int rand_state = SEED*world_rank;
        for(int i=0 ; i<niter ; ++i){
            double x = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
            double y = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
            double dist = x*x + y*y;
            if(dist<=1){
                total_count+=1;
         }
    }
    }
    if (world_rank == 0)
    {
        // TODO: process PI result
        //long finalniter = 0;
        for(int i=1 ; i<world_size; ++i){
            MPI_Recv(&count,1,MPI_UNSIGNED_LONG,i, 0,MPI_COMM_WORLD, &status);
            //MPI_Recv(&recvniter[i],world_size,MPI_LONG,MPI_ANY_SOURCE,2,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_count+=count;
        }
        
        // #pragma omp for reduction( +:finalcount)
        // for(i = 0; i<world_size; ++i)
        // {
        //     finalcount += recieved[i];
        //     //finalniter += recvniter[i];
        // }
        pi_result = ((double)total_count/(double)tosses)*4.0;

        // --- DON'T TOUCH ---
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("MPI running time: %lf Seconds\n", end_time - start_time);
        // ---
    }

    MPI_Finalize();
    return 0;
}

