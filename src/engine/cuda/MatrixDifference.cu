#pragma CUDA_MATRIX_DIFFERENCE

#include <cuda.h>
#include <stdio.h>
#include <assert.h>

#include "MatrixDifference.cuh"

#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true)
{
   if (code != cudaSuccess) 
   {
      fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
      if (abort) exit(code);
   }
}

__global__ void L1Distance(int* query, int* dataset, int* distances, int queryRowCount, int datasetRowCount, int columnCount) {
	int row = threadIdx.x + blockDim.x * blockIdx.x;
	
	int queryIndex = row % queryRowCount;
	assert(queryIndex < queryRowCount);
	int datasetIndex = row % datasetRowCount;
	assert(datasetIndex < datasetRowCount);

	if (row < queryRowCount * datasetRowCount * columnCount) {
		for (int diffIdx = 0; diffIdx < columnCount; diffIdx++) {
			distances[row] = dataset[datasetIndex + diffIdx] - query[queryIndex + diffIdx];
		}
	}
}

void L1DistanceRunner()
{
	const int queryRowCount = 16;
	const int datasetRowCount = 2*queryRowCount;
	const int rowCountDistances = queryRowCount * datasetRowCount;
	const int columnCount = 16;

	const int querySize = queryRowCount * columnCount;
	int* query = new int[querySize];
	for (int i = 0; i < querySize; i++) {
		query[i] = 10;
	}

	const int datasetSize = datasetRowCount * columnCount;
	int* dataset = new int[datasetSize];
	for (int i = 0; i < datasetSize; i++) {
		dataset[i] = 20;
	}

	const int distancesSize = rowCountDistances * columnCount;
	int* distances = new int[distancesSize];
	int* correctDistances = new int[distancesSize];

	for (int i = 0; i < rowCountDistances; i++) {
		for (int j = 0; j < queryRowCount; j++) {
			for (int k = 0; k < datasetRowCount; k++) {
				for (int p = 0; p < columnCount; p++) {
					correctDistances[i + p] = dataset[k * datasetRowCount + p] - query[j * queryRowCount + p];
				}
			}
		}
	}

	int* deviceQuery;
	gpuErrchk(cudaMalloc(&deviceQuery, sizeof(int) * querySize));
	gpuErrchk(cudaMemcpy(deviceQuery, query, sizeof(int) * querySize, cudaMemcpyHostToDevice));

	int* deviceDataset;
	gpuErrchk(cudaMalloc(&deviceDataset, sizeof(int) * datasetSize));
	gpuErrchk(cudaMemcpy(deviceDataset, dataset, sizeof(int) * datasetSize, cudaMemcpyHostToDevice));

	int* deviceDistances;
	gpuErrchk(cudaMalloc(&deviceDistances, sizeof(int) * distancesSize));

	const int blockSize = 1024;
	const int gridRows = (distancesSize + blockSize - 1) / blockSize;
	dim3 gridDim(gridRows);
	dim3 blockDim(blockSize);
	printf("gridDim: (%d)\n", gridRows);
	printf("blockDim: (%d)\n", blockSize);

	L1Distance<<<gridDim, blockDim>>>(deviceQuery, deviceDataset, deviceDistances, queryRowCount, datasetRowCount, columnCount);
	gpuErrchk(cudaPeekAtLastError());
	gpuErrchk(cudaDeviceSynchronize());
	gpuErrchk(cudaMemcpy(distances, deviceDistances, sizeof(int) * distancesSize, cudaMemcpyDeviceToHost));

	cudaFree(deviceQuery);
	cudaFree(deviceDataset);
	cudaFree(deviceDistances);
}
