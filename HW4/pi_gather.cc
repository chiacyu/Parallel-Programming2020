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
    int send_buf[1];
    int *receive_buf;

    // TODO: MPI init
    MPI_Comm_size(MPI_COMM_WORLD,&world_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
    int niter = tosses/world_size;
    //printf("The niter is %d\n",niter);

    // TODO: use MPI_Gather
    srand(SEED+world_rank);
    unsigned int rand_state = SEED*world_rank;

    for(int i=0 ; i<niter ; ++i){
        double x = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
        double y = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
        double dist = x*x + y*y;
        if(dist<1){
            send_buf[0]+=1;
        }
    }
    if(world_rank == 0){
        receive_buf=(int *)malloc(world_size*sizeof(int));
    }

    MPI_Gather(send_buf,1,MPI_INT,receive_buf,1,MPI_INT,0,MPI_COMM_WORLD);




    if (world_rank == 0)
    {
        // TODO: PI result
        int final_count=0;
        for(int i=0 ; i<world_size ; i++){
            final_count+=receive_buf[i];
        }
        //printf("The final count is %d\n",final_count);
        pi_result = ((double)final_count/(double)tosses)*4.0;

        // --- DON'T TOUCH ---
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("MPI running time: %lf Seconds\n", end_time - start_time);
        // ---
    }
    
    MPI_Finalize();
    return 0;
}
