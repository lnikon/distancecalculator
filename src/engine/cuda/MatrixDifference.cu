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

	if (row < queryRowCount) {
		for (int dRow = 0; dRow < datasetRowCount; dRow++) {
			for (int column = 0; column < columnCount; ++column) {
				int dcc = dRow * columnCount + column;
				int qcc = row * columnCount + column;
				int drcc = row * queryRowCount * columnCount + dcc;
				distances[drcc] = dataset[dcc] - query[qcc];
			}
		}
	}
}

__global__ void L2Distance(float* query, float* dataset, float* distances, int queryRowCount, int datasetRowCount, int columnCount) {
	int row = threadIdx.x + blockDim.x * blockIdx.x;

	if (row < queryRowCount) {
		for (int dRow = 0; dRow < datasetRowCount; dRow++) {
			for (int column = 0; column < columnCount; ++column) {
				int dcc = dRow * columnCount + column;
				int qcc = row * columnCount + column;
				int drcc = row * queryRowCount * columnCount + dcc;
				int imm = dataset[dcc] - query[qcc];
				distances[drcc] = imm * imm;
			}
		}
	}
}

__global__ void HammingDistance(float* query, float* dataset, float* distances, int queryRowCount, int datasetRowCount, int columnCount) {
	int row = threadIdx.x + blockDim.x * blockIdx.x;

	if (row < queryRowCount) {
		for (int dRow = 0; dRow < datasetRowCount; dRow++) {
			for (int column = 0; column < columnCount; ++column) {
				int dcc = dRow * columnCount + column;
				int qcc = row * columnCount + column;
				int drcc = row * queryRowCount * columnCount + dcc;
				int imm = dataset[dcc] - query[qcc];
				distances[drcc] = imm * imm;
			}
		}
	}
}

void L1DistanceRunner(float* query, float* dataset, float* distances, int queryRowCount, int datasetRowCount, int columnCount)
{
	const int rowCountDistances = queryRowCount * datasetRowCount;
	const int querySize = queryRowCount * columnCount;
	const int datasetSize = datasetRowCount * columnCount;
	const int distancesSize = rowCountDistances * columnCount;

	float* deviceQuery;
	gpuErrchk(cudaMalloc(&deviceQuery, sizeof(float) * querySize));
	gpuErrchk(cudaMemcpy(deviceQuery, query, sizeof(float) * querySize, cudaMemcpyHostToDevice));

	float* deviceDataset;
	gpuErrchk(cudaMalloc(&deviceDataset, sizeof(float) * datasetSize));
	gpuErrchk(cudaMemcpy(deviceDataset, dataset, sizeof(float) * datasetSize, cudaMemcpyHostToDevice));

	float* deviceDistances;
	gpuErrchk(cudaMalloc(&deviceDistances, sizeof(float) * distancesSize));

	const int blockSize = 1024;
	const int gridRows = (rowCountDistances + blockSize - 1) / blockSize;
	dim3 gridDim(gridRows);
	dim3 blockDim(blockSize);

	L1Distance<<<gridDim, blockDim>>>(deviceQuery, deviceDataset, deviceDistances, queryRowCount, datasetRowCount, columnCount);
	gpuErrchk(cudaPeekAtLastError());
	gpuErrchk(cudaDeviceSynchronize());
	gpuErrchk(cudaMemcpy(distances, deviceDistances, sizeof(float) * distancesSize, cudaMemcpyDeviceToHost));

	cudaFree(deviceQuery);
	cudaFree(deviceDataset);
	cudaFree(deviceDistances);
}

void L2DistanceRunner(float* query, float* dataset, float* distances, int queryRowCount, int datasetRowCount, int columnCount)
{
	const int rowCountDistances = queryRowCount * datasetRowCount;
	const int querySize = queryRowCount * columnCount;
	const int datasetSize = datasetRowCount * columnCount;
	const int distancesSize = rowCountDistances * columnCount;

	float* deviceQuery;
	gpuErrchk(cudaMalloc(&deviceQuery, sizeof(float) * querySize));
	gpuErrchk(cudaMemcpy(deviceQuery, query, sizeof(float) * querySize, cudaMemcpyHostToDevice));

	float* deviceDataset;
	gpuErrchk(cudaMalloc(&deviceDataset, sizeof(float) * datasetSize));
	gpuErrchk(cudaMemcpy(deviceDataset, dataset, sizeof(float) * datasetSize, cudaMemcpyHostToDevice));

	float* deviceDistances;
	gpuErrchk(cudaMalloc(&deviceDistances, sizeof(float) * distancesSize));

	const int blockSize = 1024;
	const int gridRows = (rowCountDistances + blockSize - 1) / blockSize;
	dim3 gridDim(gridRows);
	dim3 blockDim(blockSize);

	L2Distance<<<gridDim, blockDim>>>(deviceQuery, deviceDataset, deviceDistances, queryRowCount, datasetRowCount, columnCount);
	gpuErrchk(cudaPeekAtLastError());
	gpuErrchk(cudaDeviceSynchronize());
	gpuErrchk(cudaMemcpy(distances, deviceDistances, sizeof(float) * distancesSize, cudaMemcpyDeviceToHost));

	cudaFree(deviceQuery);
	cudaFree(deviceDataset);
	cudaFree(deviceDistances);
}

void HammingDistanceRunner(float* query, float* dataset, float* distances, int queryRowCount, int datasetRowCount, int columnCount)
{
	const int rowCountDistances = queryRowCount * datasetRowCount;
	const int querySize = queryRowCount * columnCount;
	const int datasetSize = datasetRowCount * columnCount;
	const int distancesSize = rowCountDistances * columnCount;

	float* deviceQuery;
	gpuErrchk(cudaMalloc(&deviceQuery, sizeof(float) * querySize));
	gpuErrchk(cudaMemcpy(deviceQuery, query, sizeof(float) * querySize, cudaMemcpyHostToDevice));

	float* deviceDataset;
	gpuErrchk(cudaMalloc(&deviceDataset, sizeof(float) * datasetSize));
	gpuErrchk(cudaMemcpy(deviceDataset, dataset, sizeof(float) * datasetSize, cudaMemcpyHostToDevice));

	float* deviceDistances;
	gpuErrchk(cudaMalloc(&deviceDistances, sizeof(float) * distancesSize));

	const int blockSize = 1024;
	const int gridRows = (rowCountDistances + blockSize - 1) / blockSize;
	dim3 gridDim(gridRows);
	dim3 blockDim(blockSize);

	HammingDistance<<<gridDim, blockDim>>>(deviceQuery, deviceDataset, deviceDistances, queryRowCount, datasetRowCount, columnCount);
	gpuErrchk(cudaPeekAtLastError());
	gpuErrchk(cudaDeviceSynchronize());
	gpuErrchk(cudaMemcpy(distances, deviceDistances, sizeof(float) * distancesSize, cudaMemcpyDeviceToHost));

	cudaFree(deviceQuery);
	cudaFree(deviceDataset);
	cudaFree(deviceDistances);
}
