#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
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

    MPI_Win win;
    srand(SEED+world_rank);
    unsigned int rand_state = SEED*world_rank;

    // TODO: MPI init
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int niter = tosses/world_size;
    int global_count = 0;
    int local_count = 0;
    int *recive_buff;

    // printf("niter is %d\n",niter);
    // printf("global_count is %d\n",global_count);
    // printf("local_count is %d\n",local_count);


    // ---
    if (world_rank == 0)
    {
        // Master
        //recive_buff = (int *)malloc(world_size * sizeof(int));
        MPI_Alloc_mem((world_size-1) * sizeof(int), MPI_INFO_NULL, &recive_buff);
        for (int i = 0; i < world_size-1; ++i)
            recive_buff[i]=0;
        MPI_Win_create(recive_buff, (world_size-1) * sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
        

        srand(SEED+world_rank);
        unsigned int rand_state = SEED*world_rank;

        for(int i=0 ; i<niter ; ++i){
        double x = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
        double y = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
        double dist = x*x + y*y;
        if(dist<=1){
            global_count+=1;
        }
        }


    int flag = 0;
    while(flag!=world_size-1){
        flag = 0;
        for(int i=0 ; i<world_size-1 ; ++i){
            MPI_Win_lock(MPI_LOCK_SHARED, 0, 0, win);
            if(recive_buff[i]!=0) flag+=1;
            MPI_Win_unlock(0, win);

        }
    }

    }
    else
    {
        // Workers

        srand(SEED+world_rank);
        unsigned int rand_state = SEED*world_rank;

        for(int i=0 ; i<niter ; ++i){
        double x = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
        double y = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
        double dist = x*x + y*y;
        if(dist<=1){
            local_count+=1;
        }
        }


        MPI_Win_create(NULL, 0, 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win);

        // Register with the master
        MPI_Win_lock(MPI_LOCK_SHARED, 0, 0, win);
        MPI_Put(&local_count, 1, MPI_INT, 0, world_rank - 1, 1, MPI_INT, win);
        MPI_Win_unlock(0, win);
    }

    
    MPI_Win_free(&win);
    

    if (world_rank == 0)
    {
        for(int i=0 ; i<world_size-1 ; ++i){
            global_count+=recive_buff[i];
        }
        // TODO: handle PI result
        pi_result = ((double)global_count/(double)tosses)*4.0;
        // --- DON'T TOUCH ---
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("MPI running time: %lf Seconds\n", end_time - start_time);
        // ---
        MPI_Free_mem(recive_buff);
    }
    
    MPI_Finalize();
    return 0;
}