#include <omp.h>

#include <cstdio>
int main() {
  printf("Hello world!\n");
  int omp_rank, n_threads;
#pragma omp parallel private(omp_rank, n_threads)
  {
    omp_rank = omp_get_thread_num();
    n_threads = omp_get_num_threads();
    printf("I'm thread %d out of %d\n", omp_rank, n_threads);
  }
  return 0;
}
