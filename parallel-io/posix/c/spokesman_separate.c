#include <errno.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATASIZE 64
#define WRITER_ID 0

void many_writers(int, int *, int);

int main(int argc, char *argv[]) {
  int my_id, ntasks, i, localsize;
  int *localvector;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  if (ntasks > 64) {
    fprintf(stderr,
            "Datasize (64) should be divisible by number "
            "of tasks.\n");
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }

  if (DATASIZE % ntasks != 0) {
    fprintf(stderr,
            "Datasize (64) should be divisible by number "
            "of tasks.\n");
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }

  localsize = DATASIZE / ntasks;
  localvector = (int *)malloc(localsize * sizeof(int));

  for (i = 0; i < localsize; i++) {
    localvector[i] = i + 1 + localsize * my_id;
  }

  many_writers(my_id, localvector, localsize);

  free(localvector);

  MPI_Finalize();
  return 0;
}

void many_writers(int my_id, int *localvector, int localsize) {
  FILE *fp;
  char filename[64];

  /* TODO: Implement a function that will write the data to file so that
     a single process does the file io. Use rank WRITER_ID as the io rank */

  sprintf(filename, "manywriters-%d.dat", my_id);

  if ((fp = fopen(filename, "wb")) == NULL) {
    fprintf(stderr, "Error: %d (%s)\n", errno, strerror(errno));
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  } else {
    fwrite(localvector, sizeof(int), DATASIZE, fp);
    fclose(fp);
    printf("Wrote %d elements to file manywriters-%d.dat\n", DATASIZE, my_id);
  }
}