#include <mpi.h>

#include <cmath>
#include <cstdio>

constexpr int n = 840;

int main(int argc, char** argv) {
  int rank;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int istart = 1;
  int istop = n;

  if (rank == 0) {
    double partial_sum = 0.0;
    printf("Computing approximation to pi with N=%d\n", n);
    istop = n / 2;
    for (int i = 1; i <= istop; i++) {
      double x = (i - 0.5) / n;
      partial_sum += 1.0 / (1.0 + x * x);
    }
    double pi = partial_sum;
    MPI_Recv(&partial_sum, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &status);
    pi += partial_sum;
    pi *= 4.0 / n;
    printf("Approximate pi=%18.16f (exact pi=%10.8f)\n", pi, M_PI);
  } else if (rank == 1) {
    double partial_sum = 0.0;
    istart = n / 2 + 1;
    for (int i = istart; i <= n; i++) {
      double x = (i - 0.5) / n;
      partial_sum += 1.0 / (1.0 + x * x);
    }
    MPI_Send(&partial_sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  }

  MPI_Finalize();
}
