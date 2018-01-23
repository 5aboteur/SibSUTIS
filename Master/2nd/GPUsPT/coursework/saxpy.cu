/*
 * nvcc -std=c++11 saxpy.cu -lcublas
 */

#include <thrust/transform.h>
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/functional.h>

#include <cublas_v2.h>
#include <functional>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <cstdio>
#include <cstdlib>

#define CUDA_ERROR_CHECK(value) {                                  \
        cudaError_t _m_cudaStat = value;                          \
        if (_m_cudaStat != cudaSuccess) {                         \
                fprintf(stderr, "Error %s at line %d in file %s\n",\
                cudaGetErrorString(_m_cudaStat),                  \
                      __LINE__, __FILE__);                        \
                exit(1);                                          \
        }                                                         \
}

void fill(int N, float x_val, float y_val, float *x, float *y)
{
        for (int i = 0; i < N; ++i) { x[i] = x_val; y[i] = y_val; }
}

float test_thrust_saxpy(int N, float a, float x_val, float y_val);
float test_cublas_saxpy(int N, float a, float x_val, float y_val);
float test_native_saxpy(int N, float a, float x_val, float y_val);

/*** DEVICE ***/

__global__ void native_saxpy(int N, float a, float *x, float *y)
{
        int i = blockDim.x * blockIdx.x + threadIdx.x;
        if (i < N) { y[i] = a * x[i] + y[i]; }
}

/*** HOST ***/

int main(void)
{
        std::ofstream fout("result.txt", std::ios_base::app);

        for (int i = 10; i < 28; ++i)
        {
                int N = 1 << i;
                std::cout << N;
                fout << test_native_saxpy(N, 2.0f, 1.0f, 2.0f) << " ";
           fout << test_cublas_saxpy(N, 2.0f, 1.0f, 2.0f) << " ";
           fout << test_thrust_saxpy(N, 2.0f, 1.0f, 2.0f) << " ";
                fout << std::endl;
        }

        return 0;
}

float test_native_saxpy(int N, float a, float x_val, float y_val)
{
        float *h_x, *h_y;
        float *d_x, *d_y;

        float result = a * x_val + y_val;
        int size = N * sizeof(float);

        h_x = (float *)malloc(size);
        h_y = (float *)malloc(size);

        fill(N, x_val, y_val, h_x, h_y);

        CUDA_ERROR_CHECK(cudaMalloc(&d_x, size));
        CUDA_ERROR_CHECK(cudaMalloc(&d_y, size));

        auto t1 = std::chrono::high_resolution_clock::now();

        CUDA_ERROR_CHECK(cudaMemcpy(d_x, h_x, size, cudaMemcpyHostToDevice));
        CUDA_ERROR_CHECK(cudaMemcpy(d_y, h_y, size, cudaMemcpyHostToDevice));

        native_saxpy<<<(N + 255) / 256, 256>>>(N, a, d_x, d_y);

        cudaDeviceSynchronize();

        CUDA_ERROR_CHECK(cudaMemcpy(h_y, d_y, size, cudaMemcpyDeviceToHost));

        auto t2 = std::chrono::high_resolution_clock::now();
        auto diff_time = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);

        float maxError = 0.0f;
        for (int i = 0; i < N; ++i)
        {
                maxError = max(maxError, abs(h_y[i] - 4.0f));
        }

        printf("\nMax error: %f\n", maxError);

        CUDA_ERROR_CHECK(cudaFree(d_x));
        CUDA_ERROR_CHECK(cudaFree(d_y));

        free(h_x);
        free(h_y);

        return diff_time.count();
}

/*** cuBLAS ***/

float test_cublas_saxpy(int N, float a, float x_val, float y_val)
{
        cublasHandle_t handle;

        float *h_x, *h_y;
        float *d_x, *d_y;

        float result = a * x_val + y_val;
        int size = N * sizeof(float);

        h_x = (float *)malloc(size);
        h_y = (float *)malloc(size);

        fill(N, x_val, y_val, h_x, h_y);

        CUDA_ERROR_CHECK(cudaMalloc(&d_x, size));
        CUDA_ERROR_CHECK(cudaMalloc(&d_y, size));

        cublasCreate(&handle);

        auto t1 = std::chrono::high_resolution_clock::now();

        cublasSetVector(N, sizeof(h_x[0]), h_x, 1, d_x, 1);
        cublasSetVector(N, sizeof(h_y[0]), h_y, 1, d_y, 1);
        cublasSaxpy(handle, N, &a, d_x, 1, d_y, 1);
        cublasGetVector(N, sizeof(h_y[0]), d_y, 1, h_y, 1);

        auto t2 = std::chrono::high_resolution_clock::now();
        auto diff_time = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);

        assert(std::all_of(h_y, h_y + N, [&](float value) { return value == result; }));

        cublasDestroy(handle);

        CUDA_ERROR_CHECK(cudaFree(d_x));
        CUDA_ERROR_CHECK(cudaFree(d_y));

       free(h_x);
        free(h_y);

        return diff_time.count();
}

/*** THRUST ***/

struct saxpy_functor : public thrust::binary_function<float, float, float>
{
        const float a;
        saxpy_functor(float _a) : a(_a) {}

        __host__ __device__ float operator()(const float &x, const float &y) const
        {
                return a * x + y;
        }
};

float test_thrust_saxpy(int N, float a, float x_val, float y_val)
{
        float result = a * x_val + y_val;

        thrust::host_vector<float> h_x(N), h_y(N);
        thrust::fill(h_x.begin(), h_x.end(), x_val);
        thrust::fill(h_y.begin(), h_y.end(), y_val);

        auto t1 = std::chrono::high_resolution_clock::now();
        thrust::device_vector<float> d_x = h_x;
        thrust::device_vector<float> d_y = h_y;
        thrust::transform(d_x.begin(), d_x.end(), d_y.begin(), d_y.begin(), saxpy_functor(a));

        h_y = d_y;

        auto t2 = std::chrono::high_resolution_clock::now();
        auto diff_time = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
        assert(std::all_of(h_y.cbegin(), h_y.cend(), [&](float value) { return value == result; }));

        return diff_time.count();
}
