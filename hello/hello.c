#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
   int idp; // Process ID
   int np;  // Number of processors

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &idp);
   MPI_Comm_size(MPI_COMM_WORLD, &np);

   printf("Hello from process %d/%d\n", idp, np);

   MPI_Finalize();

   return 0;
}
