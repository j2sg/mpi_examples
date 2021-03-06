/**
 *  This file is part of MPI_Examples.
 *
 *  Copyright (c) 2014 Juan Jose Salazar Garcia jjslzgc@gmail.com
 *
 *  MPI_Examples is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  MPI_Examples is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with MPI_Examples.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

/*
 *   Build: mpicc -o send_recv send_recv.c
 *   Run:   mpirun -np <Number of processors> send_recv
 */

#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define MAXSTRING 128


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

