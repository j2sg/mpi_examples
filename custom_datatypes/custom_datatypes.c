#include <stdio.h>
#include <mpi.h>

/*
 *   Build: mpicc -o custom_datatypes custom_datatypes.c
 *   Run:   mpirun -np <Number of processors> custom_datatypes
 */

#define VECTOR_BLOCKS 1
#define PARTICLE_BLOCKS 2

// Declaration of datatypes

// Vector in vector space R3
typedef struct VectorType {
    double x;
    double y;
    double z;
} Vector;

// Particle in vector space R3
typedef struct ParticleType {
    double mass;
    Vector position;
    Vector speed;
    Vector acceleration;
} Particle;

int main(int argc, char *argv[])
{
   int idp;              // Process ID
   int np;               // Number of processors

   // MPI Initialization
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &idp);
   MPI_Comm_size(MPI_COMM_WORLD, &np);

   // MPI Datatypes creation

   // MPI Vector datatype
   MPI_Datatype MPI_VECTOR;
   int vectorBlockLength[VECTOR_BLOCKS] = {3};
   MPI_Aint vectorOffset[VECTOR_BLOCKS] = {0};
   MPI_Datatype vectorOldType[VECTOR_BLOCKS] = {MPI_DOUBLE};
   MPI_Type_struct(VECTOR_BLOCKS, vectorBlockLength, vectorOffset, vectorOldType, &MPI_VECTOR);
   MPI_Type_commit(&MPI_VECTOR);

   // MPI Particle datatype
   MPI_Datatype MPI_PARTICLE;
   int particleBlockLength[PARTICLE_BLOCKS] = {1, 3};
   MPI_Aint doubleExtent;
   MPI_Type_extent(MPI_DOUBLE, &doubleExtent);
   MPI_Aint particleOffset[PARTICLE_BLOCKS] = {0, doubleExtent};
   MPI_Datatype particleOldType[PARTICLE_BLOCKS] = {MPI_DOUBLE, MPI_VECTOR};
   MPI_Type_struct(PARTICLE_BLOCKS, particleBlockLength, particleOffset, particleOldType, &MPI_PARTICLE);
   MPI_Type_commit(&MPI_PARTICLE);

   MPI_Status status;

   // Process 0 receives particles from other processes
   if(idp == 0) {
      while(--np) {
         Particle particleReceived;

         MPI_Recv(&particleReceived, 1, MPI_PARTICLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
         printf("Particle from %d [m=%.3lf p=%.2lfi+%.2lfj+%.2lfk v=%.2lfi+%.2lfj+%.2lfk a=%.2lfi+%.2lfj+%.2lfk]\n",
                status.MPI_SOURCE, particleReceived.mass,
                particleReceived.position.x, particleReceived.position.y, particleReceived.position.z,
                particleReceived.speed.x, particleReceived.speed.y, particleReceived.speed.z,
                particleReceived.acceleration.x, particleReceived.acceleration.y, particleReceived.acceleration.z);
      }
   } else {
      Particle particleSent;

      particleSent.mass = idp / 10.0;
      particleSent.position.x = particleSent.position.y = particleSent.position.z = idp;
      particleSent.speed.x = particleSent.speed.y = particleSent.speed.z = idp * 10;
      particleSent.acceleration.x = particleSent.acceleration.y = particleSent.acceleration.z = idp * 100;

      MPI_Send(&particleSent, 1, MPI_PARTICLE, 0, 0, MPI_COMM_WORLD);
   }



   // Free MPI resources

   MPI_Type_free(&MPI_PARTICLE);
   MPI_Type_free(&MPI_VECTOR);
   MPI_Finalize();

   return 0;
}

