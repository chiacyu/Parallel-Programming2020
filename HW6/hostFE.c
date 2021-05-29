#include <stdio.h>
#include <stdlib.h>
#include "hostFE.h"
#include "helper.h"

#include <stdio.h>
#include <stdlib.h>
#include "hostFE.h"
#include "helper.h"

#define WGX 16
#define WGY 16

unsigned int roundUp(unsigned int value, unsigned int multiple){
    unsigned int remainder = value%multiple;
    if(remainder!=0){
        value+=(multiple-remainder);
    }
    return value;
}

void hostFE(int filterWidth, 
            float *filter,
            int imageHeight,
            int imageWidth,
            float *inputImage,
            float *outputImage,
            cl_device_id *device,
            cl_context *context,
            cl_program *program)
{
    cl_int status;
    int filterSize = filterWidth * filterWidth;
    int dataSize = imageHeight * imageWidth *sizeof(float);
    //
    int deviceWidth = imageWidth;

    int deviceHeight = imageHeight;
    int deviceDataSize = imageHeight * deviceWidth * sizeof(float);


    //filter initialize
    int filterRadius = filterWidth/2;
    int paddingPixels = (int)(filterWidth/2)*2;

    //Setup OpenCl Environment

    //Find is there active platform


    //create queue
    cl_command_queue queue = clCreateCommandQueue(*context, *device, 0, NULL);
    //Create memory buffer

    cl_mem d_inputImage;
    cl_mem d_outputImage;
    cl_mem d_filter;

    d_inputImage = clCreateBuffer(*context,CL_MEM_READ_ONLY, deviceDataSize, NULL, NULL);
    d_outputImage = clCreateBuffer(*context,CL_MEM_WRITE_ONLY, deviceDataSize, NULL, NULL);
    d_filter = clCreateBuffer(*context,CL_MEM_READ_ONLY, filterSize * sizeof(float), NULL, NULL);
    
    //write inputdata to device

    //#ifdef NON_OPTIMIZED
    clEnqueueWriteBuffer(queue, d_inputImage, CL_TRUE, 0, deviceDataSize, inputImage,0,NULL,NULL);


    //write filter to device
    clEnqueueWriteBuffer(queue, d_filter, CL_TRUE, 0, filterSize*sizeof(float), filter, 0,NULL,NULL);
    


    // //create kernel

    

    // //only the host-side code differs for the aligned reads
    cl_kernel kernel;
    kernel = clCreateKernel(*program, "convolution", NULL);

    //select workgroup size is 16*16
    int wgWidth = WGX;
    int wgHeight = WGY;

    //padding number doesn't need to be consider
    int totalWorkItemsX = roundUp(imageWidth-paddingPixels,wgWidth);
    int totalWorkItemsY = roundUp(imageHeight-paddingPixels,wgHeight);

    //size of workgroup
    size_t localSize[2] = {wgWidth, wgHeight};
    //size of the NDrange
    size_t globalSize[2] = {totalWorkItemsX, totalWorkItemsY};

    //The amount of local data that is cached is the size of the workgroup plus the padding pixels

    // #if defined NON_OPTIMIZED || defined READ_ALLIGNED
    int localWidth = localSize[0]+paddingPixels;

    // #endif
    int localHeight = localSize[1]+paddingPixels;
    //compute the size of local memory

    size_t localMemSize=(localWidth*localHeight*sizeof(float));

    // //set the kernel arguments
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_inputImage);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_outputImage);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_filter);
    clSetKernelArg(kernel, 3, sizeof(int), &deviceHeight);
    clSetKernelArg(kernel, 4, sizeof(int), &deviceWidth);
    clSetKernelArg(kernel, 5, sizeof(int), &filterWidth);
    clSetKernelArg(kernel, 6, localMemSize, NULL);
    clSetKernelArg(kernel, 7, sizeof(int), &localHeight);
    clSetKernelArg(kernel, 8, sizeof(int), &localWidth);
    // //Execute the kernel
    clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalSize, localSize, 0, NULL, NULL);
    clFinish(queue);

    clEnqueueReadBuffer(queue, d_outputImage, CL_TRUE, 0, deviceDataSize, outputImage, 0, NULL, NULL);

    //Free OpenCL objects
    return 0;

}
