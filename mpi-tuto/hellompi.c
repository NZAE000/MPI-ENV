#include<stdio.h>                                   
#include<mpi.h>                                         // We include the MPI header here to have access to the various MPI functions.

int 
main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);                               // Here we initialize the MPI execution environment. This must be done at the start of the program.
    int rank = 0, size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);               // Each MPI process is assigned a unique integer ID starting at 0. This function retrieves the local ID and stores it in the local variable rank.
    MPI_Comm_size(MPI_COMM_WORLD, &size);               // This function retrieves the total number of MPI processes running and stores it in the local variable size.
    printf("Hello from process %d/%d\n", rank, size);   // Here we print hello world with the environment information we gathered in the previous lines.
    
    MPI_Finalize();                                     // Here we finalize the MPI execution environment. This must be done before the end of the program.

    return 0;
}
