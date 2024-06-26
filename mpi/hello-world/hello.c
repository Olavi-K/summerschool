#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  int size, rank, resultlen;
  char name[MPI_MAX_PROCESSOR_NAME];
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Get_processor_name(name, &resultlen);

  // TODO: say hello! in parallel
  if (rank == 0) {
    printf("Hello from rank %d and processor %s. The total size is %d.\n", rank,
           name, size);
  } else {
    printf("Hello from rank %d and processor %s.\n", rank, name);
  }
  MPI_Finalize();
}
