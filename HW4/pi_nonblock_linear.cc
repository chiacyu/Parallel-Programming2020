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
    double pi_result = 0;
    long long int tosses = atoi(argv[1]);
    int world_rank, world_size;
    int total_count = 0;
    int count = 0;

    

    int *recieved;
    recieved = (int *) malloc (sizeof(int) * 1);
    
    //TODO: MPI init
    MPI_Comm_size(MPI_COMM_WORLD,&world_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
    
    srand(time(NULL));
    unsigned int rand_state = SEED*world_rank;
    int niter = tosses/world_size;
    printf("the niter number is %d\n",niter);

    if (world_rank > 0)
    {
        // TODO: MPI workers
        unsigned int rand_state = SEED*world_rank;
        for(int i=0 ; i<niter ; ++i){
            double x = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
            double y = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
            double dist = (x*x) + (y*y);
            //printf("The dist number is %f\n",dist);
            if(dist<=1){
                count+=1;
            }
        }
        recieved[0] = count;
        printf("The count is %d\n",recieved[0]);
        MPI_Send(&recieved,1,MPI_INT,0,0,MPI_COMM_WORLD);
        
        
    }
    else if (world_rank == 0)
    {
        MPI_Request requests[world_size-1];
    MPI_Status status[world_size-1];
        // TODO: non-blocking MPI communication.
        // Use MPI_Irecv, MPI_Wait or MPI_Waitall.
        //unsigned int rand_state = SEED*world_rank;

        for(int i=0 ; i<niter ; ++i){
            double x = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
            double y = rand_r(&rand_state) / ((double)RAND_MAX + 1) * 2.0 - 1.0;
            double dist = (x*x) + (y*y);
            if(dist<1){
                total_count+=1;
            }
        }
        recieved[0] = total_count;
for(int i=1 ; i<world_size ; ++i){
            // int recieved_count[1];
            // recieved_count[0] = 0;
            MPI_Irecv(&recieved, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &requests[i]);
            total_count+=recieved[0];
            //printf("the total count is %d\n",recieved[0]);
        }


        MPI_Waitall(0,requests,status);
    }

    if (world_rank == 0)
    {

        

        
        // TODO: PI result
        pi_result = 4.0*total_count/(double)tosses;
        printf("the total count is %d\n",total_count);

        // --- DON'T TOUCH ---
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("MPI running time: %lf Seconds\n", end_time - start_time);
        // ---
    }

    MPI_Finalize();
    return 0;
}
