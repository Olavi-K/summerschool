#include <hip/hip_runtime.h>
#include <math.h>
#include <stdio.h>

// TODO: add a device kernel that copies all elements of a vector
//       using GPU threads in a 2D grid
__global__ void copy2d_(int n, int m, double *source, double *target) {
  int tidx = threadIdx.x + blockIdx.x * blockDim.x;
  int stridex = gridDim.x * blockDim.x;
  int tidy = threadIdx.y + blockIdx.y * blockDim.y;
  int stridey = gridDim.y * blockDim.y;

  for (; tidx < n; tidx += stridex) {
    for (; tidy < n; tidy += stridey) {
      target[tidx * m + tidy] = source[tidx * m + tidy];
    }
  }
}

int main(void) {
  int i, j;
  const int n = 600;
  const int m = 400;
  const int size = n * m;
  double x[size], y[size], y_ref[size];
  double *x_, *y_;

  // initialise data
  for (i = 0; i < size; i++) {
    x[i] = (double)i / 1000.0;
    y[i] = 0.0;
  }
  // copy reference values (C ordered)
  for (i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      y_ref[i * m + j] = x[i * m + j];
    }
  }

  // TODO: allocate vectors x_ and y_ on the GPU
  hipMalloc(&x_, sizeof(double) * size);
  hipMalloc(&y_, sizeof(double) * size);
  // TODO: copy initial values from CPU to GPU (x -> x_ and y -> y_)
  hipMemcpy(x_, x, sizeof(double) * size, hipMemcpyHostToDevice);
  hipMemcpy(y_, y, sizeof(double) * size, hipMemcpyHostToDevice);

  // TODO: define grid dimensions (use 2D grid!)
  dim3 blocks(10, 12);
  dim3 threads(64, 4);
  // TODO: launch the device kernel
  hipLaunchKernelGGL(copy2d_, blocks, threads, 0, 0, n, m, x_, y_);

  // TODO: copy results back to CPU (y_ -> y)
  hipMemcpy(y, y_, sizeof(double) * size, hipMemcpyDeviceToHost);

  // confirm that results are correct
  double error = 0.0;
  for (i = 0; i < size; i++) {
    error += abs(y_ref[i] - y[i]);
  }
  printf("total error: %f\n", error);
  printf("  reference: %f at (42,42)\n", y_ref[42 * m + 42]);
  printf("     result: %f at (42,42)\n", y[42 * m + 42]);

  return 0;
}
