#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void random_ints(int* a, const int n);
void print(int* a, const int n);

#define TOTAL 32
#define BLOCKS 8
#define THREADS 4

#define BLOCK_SIZE 8
#define RADIUS 3

__global__ void add(int* a, int* b, int* c, const int n)
{
    int index = threadIdx.x + blockIdx.x * blockDim.x;
    if (index < n)
    {
        c[index] = a[index] + b[index];
    }
}

__global__ void stencil_1d(int* in, int* out)
{
    __shared__ int temp[BLOCK_SIZE + 2 * RADIUS];
    int            gindex = threadIdx.x + blockIdx.x * blockDim.x;
    int            lindex = threadIdx.x + RADIUS;

    temp[lindex] = in[gindex];
    if (threadIdx.x < RADIUS)
    {
        temp[lindex - RADIUS]     = in[gindex - RADIUS];
        temp[lindex + BLOCK_SIZE] = in[gindex + BLOCK_SIZE];
    }

    __syncthreads();

    int result = 0;
    for (int offset = -RADIUS; offset <= RADIUS; ++offset)
    {
        result += temp[lindex + offset];
    }

    out[gindex] = result;
}

int main(void)
{
    srand(time(NULL));

    int *     a, *b, *c;
    int *     d_a, *d_b, *d_c;
    const int size = TOTAL * sizeof(int);

    cudaMalloc((void**)&d_a, size);
    cudaMalloc((void**)&d_b, size);
    cudaMalloc((void**)&d_c, size);

    a = (int*)malloc(size);
    random_ints(a, TOTAL);
    b = (int*)malloc(size);
    random_ints(b, TOTAL);
    c = (int*)malloc(size);

    cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);

    const int threadCount = THREADS;
    const int blockCount  = TOTAL / threadCount;

    std::cout << "\nBlockCount: " << blockCount << "\nThreadCount: " << threadCount << "\n";

    add<<<blockCount, threadCount>>>(d_a, d_b, d_c, TOTAL);

    cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);

    print(a, TOTAL);
    print(b, TOTAL);
    print(c, TOTAL);

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    free(a);
    free(b);
    free(c);

    return 0;
}

void random_ints(int* a, const int n)
{
    for (int i = 0; i < n; ++i)
        a[i] = rand() % 100;
}

void print(int* a, const int n)
{
    for (int i = 0; i < n; ++i)
        std::cout << std::setw(4) << a[i] << ' ';
    std::cout << std::endl;
}
