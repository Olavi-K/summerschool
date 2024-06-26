#include <mpi.h>

#include <cstdio>
#include <vector>

void print_ordered(double t);

int main(int argc, char *argv[]) {
  int i, rank, ntasks;
  constexpr int size = 10000000;
  std::vector<int> message(size);
  std::vector<int> receiveBuffer(size);
  MPI_Status status;

  double t0, t1;

  int source, destination;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Initialize buffers
  for (i = 0; i < size; i++) {
    message[i] = rank;
    receiveBuffer[i] = -1;
  }

  // Set source and destination ranks
  if (rank < ntasks - 1) {
    destination = rank + 1;
  } else {
    destination = MPI_PROC_NULL;
  }
  if (rank > 0) {
    source = rank - 1;
  } else {
    source = MPI_PROC_NULL;
  }

  // Start measuring the time spent in communication
  MPI_Barrier(MPI_COMM_WORLD);
  t0 = MPI_Wtime();

  // Send and receive messages
  MPI_Sendrecv(message.data(), size, MPI_INT, destination, rank + 1,
               receiveBuffer.data(), size, MPI_INT, source, MPI_ANY_TAG,
               MPI_COMM_WORLD, &status);
  printf("Sender: %d. Sent elements: %d. Tag: %d. Receiver: %d\n", rank, size,
         rank + 1, destination);
  printf("Receiver: %d. first element %d.\n", rank, receiveBuffer[0]);

  // Finalize measuring the time and print it out
  t1 = MPI_Wtime();
  MPI_Barrier(MPI_COMM_WORLD);
  fflush(stdout);

  print_ordered(t1 - t0);

  MPI_Finalize();
  return 0;
}

void print_ordered(double t) {
  int i, rank, ntasks;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

  if (rank == 0) {
    printf("Time elapsed in rank %2d: %6.3f\n", rank, t);
    for (i = 1; i < ntasks; i++) {
      MPI_Recv(&t, 1, MPI_DOUBLE, i, 11, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("Time elapsed in rank %2d: %6.3f\n", i, t);
    }
  } else {
    MPI_Send(&t, 1, MPI_DOUBLE, 0, 11, MPI_COMM_WORLD);
  }
}
