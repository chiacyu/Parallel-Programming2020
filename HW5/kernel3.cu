#include <cuda.h>
#include <stdio.h>
#include <stdlib.h>

__device__ static inline int mandel(float c_re, float c_im, int count)
{
  float z_re = c_re, z_im = c_im;
  int i;
  for (i = 0; i < count; ++i)
  {

    if (z_re * z_re + z_im * z_im > 4.f)
      break;

    float new_re = z_re * z_re - z_im * z_im;
    float new_im = 2.f * z_re * z_im;
    z_re = c_re + new_re;
    z_im = c_im + new_im;
  }

  return i;
}

//how many pixel does a thread compute

static const int thr_blk_x = 4;
static const int thr_blk_y = 4;
static const int block_side = 16;

__global__ void mandelKernel(int *d_res, float lowerX, float lowerY, float stepX, float  stepY, int resX, int resY, int pitch, int maxIterations)
{
    // To avoid error caused by the floating number, use the following pseudo code
    //
    
    int myX, myY;
    myX = (blockIdx.x*blockDim.x + threadIdx.x)*thr_blk_x;
    myY = (blockIdx.y*blockDim.y + threadIdx.y)*thr_blk_y;

    int i,j;

    for(i = myX ; i<myX+thr_blk_x ; i++){
      for(j = myY ; j<myY+thr_blk_y ; j++){
        if (i>=resX || j>=resY) continue;
        float x, y;
        x = lowerX + i * stepX;
        y = lowerY +j * stepY;
        
        int color = mandel (x, y, maxIterations) ; 
        d_res[j * resX + i] = color;
      }
    }


}
//
// Host front-end function that allocates the memory and launches the GPU kernel
void hostFE (float upperX, float upperY, float lowerX, float lowerY, int* img, int resX, int resY, int maxIterations)
{
    float stepX = (upperX - lowerX) / resX;
    float stepY = (upperY - lowerY) / resY;

    int blocksX, blocksY;
    blocksX = (int) ceil (resX*1.0 / (block_side*thr_blk_x));
    blocksY= (int)ceil(resY*1.0/ (block_side*thr_blk_y));

    dim3 block (block_side, block_side);
    dim3 grid (blocksX , blocksY);

    size_t pitch;
    int *h_res;
    int *d_res;

    cudaMallocPitch((void **)&d_res, (size_t *)&pitch, sizeof(int)*resX, resY);
    cudaHostAlloc(&h_res, resX*resY*sizeof(int), cudaHostAllocMapped);
    mandelKernel <<<grid, block>>> ( d_res ,lowerX, lowerY, stepX ,stepY, resX, resY, pitch,maxIterations);

    cudaDeviceSynchronize();
    cudaMemcpy(h_res, d_res, resY * resX * sizeof(int), cudaMemcpyDeviceToHost);

    for(int i=0 ; i<resY ; i++){
      for(int j=0 ; j<resX ; j++){
        img[i*resX + j] = h_res[i*resX+j];
      }
    }
    cudaFreeHost(h_res);
    cudaFree(d_res);
}