#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define MAXSTRING 128

/*
 *   Build: mpicc -o send_recv send_recv.c
 *   Run:   mpirun -np <Number of processors> send_recv
 */

int main(int argc, char *argv[])
{
   int idp;              // Process ID
   int np;               // Number of processors
   char msg[MAXSTRING];  // Message to be sent/received

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &idp);
   MPI_Comm_size(MPI_COMM_WORLD, &np);

   if(idp == 0) {
      while(--np) {
         MPI_Recv(msg, MAXSTRING, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         printf("%s\n", msg);
      }
   } else {
      sprintf(msg, "Hello from process %d/%d", idp, np);
      MPI_Send(msg, strlen(msg) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
   }

   MPI_Finalize();

   return 0;
}

