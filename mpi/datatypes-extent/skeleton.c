#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
  int rank, ntasks;
  int array[8][6];
  int recvarray[8][6];
  MPI_Datatype columntype;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

  // Initialize arrays
  if (rank == 0) {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 6; j++) {
        array[i][j] = (i + 1) * 10 + j + 1;
      }
    }
  } else {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 6; j++) {
        array[i][j] = 0;
      }
    }
  }

  // Print data on rank 0
  if (rank == 0) {
    printf("Data on rank %d\n", rank);
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 6; j++) {
        printf("%3d", array[i][j]);
      }
      printf("\n");
    }
  }

  // Create datatype
  // TODO
  MPI_Type_vector(8, 1, 6, MPI_INT, &columntype);
  MPI_Aint lb, extent;
  MPI_Type_get_extent(columntype, &lb, &extent);
  //   if (rank == 0) {
  //     printf("Extent before resize is %ld elements\n", extent / sizeof(int));
  //   }

  MPI_Datatype tmp = columntype;
  MPI_Type_create_resized(tmp, 0, sizeof(int), &columntype);
  MPI_Type_get_extent(columntype, &lb, &extent);
  //   MPI_Type_create_resized(columntype, 0, sizeof(int), &columntype);
  //   if (rank == 0) {
  //     printf("Extent after  resize is %ld elements\n", extent / sizeof(int));
  //   }

  MPI_Type_commit(&columntype);

  MPI_Scatter(array, 1, columntype, recvarray, 1, columntype, 0,
              MPI_COMM_WORLD);
  // Send data from rank 0 to rank 1
  // TODO
  //   if (rank == 0) {
  //     MPI_Send(array, 3, columntype, 1, 1, MPI_COMM_WORLD);
  //   } else if (rank == 1) {
  //     MPI_Recv(array, 3, columntype, 0, 1, MPI_COMM_WORLD,
  //     MPI_STATUS_IGNORE);
  //   }

  // Free datatype
  // TODO
  MPI_Type_free(&columntype);

  // Print received data
  //   if (rank == 1) {
  //     printf("Received data on rank %d\n", rank);
  //     for (int i = 0; i < 8; i++) {
  //       for (int j = 0; j < 6; j++) {
  //         printf("%3d", array[i][j]);
  //       }
  //       printf("\n");
  //     }
  //   }
  for (int irank = 0; irank < ntasks; irank++) {
    if (irank == rank) {
      printf("Received data on rank %d\n", rank);
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 6; j++) {
          printf("%3d", recvarray[i][j]);
        }
        printf("\n");
      }
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }

  MPI_Finalize();

  return 0;
}
