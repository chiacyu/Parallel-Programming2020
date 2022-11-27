#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    unsigned int seed = 23;
    int final_cnt;
    MPI_Status status;
    int number_in_circle;
    float x, y;
    long long int number_of_tosses;
    // --- DON'T TOUCH ---
    MPI_Init(&argc, &argv);
    double start_time = MPI_Wtime();
    double pi_result;
    long long int tosses = atoi(argv[1]);
    int world_rank, world_size;
    // ---

    // TODO: MPI init
    //Get process ID
    MPI_Comm_rank (MPI_COMM_WORLD, &world_rank);  
    //Get processes Number
    MPI_Comm_size (MPI_COMM_WORLD, &world_size);
    number_in_circle = 0;
    number_of_tosses = tosses / world_size;
    
    for (long long int i = 0; i < number_of_tosses ; i++) {
        x = rand_r(&seed)/(float)RAND_MAX;
        y = rand_r(&seed)/(float)RAND_MAX;
        if ( (x * x + y * y) <= 1){
            number_in_circle++;
        };
    };
    int go_num = 1;
    if(world_rank == 0){
      final_cnt = number_in_circle;
    }
    // TODO: binary tree redunction
    while(world_size / go_num != 1){
        if(world_rank == 0){
            MPI_Recv(&number_in_circle, 1, MPI_INT, go_num, 0, MPI_COMM_WORLD, &status);
            final_cnt += number_in_circle;
        }else{
              if(world_rank%(go_num*2) == 0){
                  int local_number_in_circle = number_in_circle;
                  MPI_Recv(&number_in_circle, 1, MPI_INT, world_rank + go_num, 0, MPI_COMM_WORLD, &status);
                  number_in_circle += local_number_in_circle;
                   
              }else{
                  MPI_Send(&number_in_circle, 1, MPI_INT, world_rank - go_num, 0, MPI_COMM_WORLD);
                  break;
              } 
        
        }
        go_num *= 2;
    }
    

    if (world_rank == 0)
    {
        // TODO: PI result
        pi_result = 4.0 * final_cnt/(float)tosses;
        // --- DON'T TOUCH ---
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("MPI running time: %lf Seconds\n", end_time - start_time);
        // ---
    }

    MPI_Finalize();
    return 0;
}
