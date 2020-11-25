#pragma CUDA_MATRIX_DIFFERENCE

#include <cuda.h>
#include <cuda_runtime.h>

__global__ void matrix_for_test(int* a, int* b, int* c, int m, int n, int k);
void L1DistanceRunner();
