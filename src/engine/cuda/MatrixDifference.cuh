#pragma CUDA_MATRIX_DIFFERENCE

#include <cuda.h>
#include <cuda_runtime.h>

void L1DistanceRunner(float* query, float* dataset, float* distances, int queryRowCount, int datasetRowCount, int columnCount);
void L2DistanceRunner(float* query, float* dataset, float* distances, int queryRowCount, int datasetRowCount, int columnCount);
void HammingDistanceRunner(float* query, float* dataset, float* distances, int queryRowCount, int datasetRowCount, int columnCount);

