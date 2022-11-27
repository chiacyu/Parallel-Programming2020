__kernel void convolution(
                          __global float* imageIn,
                          __global float* imageOut,
                          __constant float* filter,
                          int rows,
                          int cols,
                          int filterWidth,
                          __local float* localImage,
                          int localHeight,
                          int localWidth) 
{
    int filterRadius = (filterWidth/2);
    int padding = filterRadius * 2;

    int groupStartCol = get_group_id(0)*get_local_size(0);
    int groupStartRow = get_group_id(1)*get_local_size(1);
    
    int localCol = get_local_id(0);
    int localRow = get_local_id(1);

    int globalCol = groupStartCol+localCol;
    int globalRow = groupStartRow+localRow;

    for( int i=localRow ; i<localHeight ; i+=get_local_size(1)){
        int curRow = groupStartRow+i;
        for(int j=localCol; j<localWidth; j+=get_local_size(0)){
            int curCol = groupStartCol+j;
            if( (curRow<rows) && (curCol<cols) ){
                localImage[i * localWidth + j] = imageIn[curRow * cols + curCol];
            }
        }
    }

    barrier(CLK_LOCAL_MEM_FENCE);

    if((globalRow < (rows-padding)) && (globalCol < (cols-padding))){
        float sum = 0.0f;
        int filterIdx=0;

        for(int i=localRow ; i < localRow+filterWidth ; i++){
            int offset = i*localWidth;
            for(int j=localCol ; j<localCol+filterWidth ; j++){
                sum+=localImage[offset+j]*filter[filterIdx++];
            }
        }
    
        // for(int i=localRow ; i<localRow+filterWidth ; i++){
        //      int offset = i * localWidth + localCol;
        //      sum+=localImage[offset++]*filter[filterIdx++];
        //      sum+=localImage[offset++]*filter[filterIdx++];
        //      sum+=localImage[offset++]*filter[filterIdx++];
        //      sum+=localImage[offset++]*filter[filterIdx++];
        //      sum+=localImage[offset++]*filter[filterIdx++];
        //      sum+=localImage[offset++]*filter[filterIdx++];
        //      sum+=localImage[offset++]*filter[filterIdx++];
        //      sum+=localImage[offset++]*filter[filterIdx++];
        // }

        imageOut[(globalRow+filterRadius)*cols+(globalCol+filterRadius)] = sum;
    }
    return;
}

