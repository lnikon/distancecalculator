#pragma CUDA_MATRIX_DIFFERENCE

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

__global__ void L1Distance(float* query, float* dataset, float* distances, int queryRowCount, int datasetRowCount, int columnCount) {
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
	float* query = new float[querySize];
	for (int i = 0; i < querySize; i++) {
		query[i] = 10;
	}

	const int datasetSize = datasetRowCount * columnCount;
	float* dataset = new float[datasetSize];
	for (int i = 0; i < datasetSize; i++) {
		dataset[i] = 20;
	}

	const int distancesSize = rowCountDistances * columnCount;
	float* distances = new float[distancesSize];
	float* correctDistances = new float[distancesSize];

	for (int i = 0; i < rowCountDistances; i++) {
		for (int j = 0; j < queryRowCount; j++) {
			for (int k = 0; k < datasetRowCount; k++) {
				for (int p = 0; p < columnCount; p++) {
					correctDistances[i + p] = dataset[k * datasetRowCount + p] - query[j * queryRowCount + p];
				}
			}
		}
	}

	float* deviceQuery;
	gpuErrchk(cudaMalloc(&deviceQuery, sizeof(float) * querySize));
	gpuErrchk(cudaMemcpy(deviceQuery, query, sizeof(float) * querySize, cudaMemcpyHostToDevice));

	float* deviceDataset;
	gpuErrchk(cudaMalloc(&deviceDataset, sizeof(float) * datasetSize));
	gpuErrchk(cudaMemcpy(deviceDataset, dataset, sizeof(float) * datasetSize, cudaMemcpyHostToDevice));

	float* deviceDistances;
	gpuErrchk(cudaMalloc(&deviceDistances, sizeof(float) * distancesSize));

	const int blockSize = 1024;
	const int gridRows = (distancesSize + blockSize - 1) / blockSize;
	dim3 gridDim(gridRows);
	dim3 blockDim(blockSize);
	printf("gridDim: (%d)\n", gridRows);
	printf("blockDim: (%d)\n", blockSize);

	L1Distance<<<gridDim, blockDim>>>(deviceQuery, deviceDataset, deviceDistances, queryRowCount, datasetRowCount, columnCount);
	gpuErrchk(cudaPeekAtLastError());
	gpuErrchk(cudaDeviceSynchronize());
	gpuErrchk(cudaMemcpy(distances, deviceDistances, sizeof(float) * distancesSize, cudaMemcpyDeviceToHost));

	cudaFree(deviceQuery);
	cudaFree(deviceDataset);
	cudaFree(deviceDistances);
}
