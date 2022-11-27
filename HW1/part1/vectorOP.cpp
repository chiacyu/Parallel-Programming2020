#include "PPintrin.h"

// implementation of absSerial(), but it is vectorized using PP intrinsics
void absVector(float *values, float *output, int N)
{
  __pp_vec_float x;
  __pp_vec_float result;
  __pp_vec_float zero = _pp_vset_float(0.f);
  __pp_mask maskAll, maskIsNegative, maskIsNotNegative;

  //  Note: Take a careful look at this loop indexing.  This example
  //  code is not guaranteed to work when (N % VECTOR_WIDTH) != 0.
  //  Why is that the case?
  for (int i = 0; i < N; i += VECTOR_WIDTH)
  {

    // All ones
    maskAll = _pp_init_ones();

    // All zeros
    maskIsNegative = _pp_init_ones(0);

    // Load vector of values from contiguous memory addresses
    _pp_vload_float(x, values + i, maskAll); // x = values[i];

    // Set mask according to predicate
    _pp_vlt_float(maskIsNegative, x, zero, maskAll); // if (x < 0) {

    // Execute instruction using mask ("if" clause)
    _pp_vsub_float(result, zero, x, maskIsNegative); //   output[i] = -x;

    // Inverse maskIsNegative to generate "else" mask
    maskIsNotNegative = _pp_mask_not(maskIsNegative); // } else {

    // Execute instruction ("else" clause)
    _pp_vload_float(result, values + i, maskIsNotNegative); //   output[i] = x; }

    // Write results back to memory
    _pp_vstore_float(output + i, result, maskAll);
  }
}



void clampedExpVector(float* values, int* exponents, float* output, int N) {
    //init the vector for calculation
  __pp_vec_float x;
  __pp_vec_int y;
  __pp_vec_float result;
  __pp_vec_float clamps;

  __pp_vec_int zeros = _pp_vset_int(0);
  __pp_vec_int ones = _pp_vset_int(1);

  __pp_mask maskAll, maskDone;
  int n = N/VECTOR_WIDTH;
  int m = N%VECTOR_WIDTH;

  if (m > 0)
    n += 1;

  for (int i=0; i<n; ++i) {

    int lanes = VECTOR_WIDTH;
    if (i==n-1 && m > 0)
      lanes = m;

    maskAll = _pp_init_ones(lanes);
    maskDone = _pp_mask_not(maskAll);

    _pp_vload_float(x, values+i*lanes, maskAll);
    _pp_vload_int(y, exponents+i*lanes, maskAll);

    _pp_vset_float(result, 1.0f, maskAll);
    _pp_vset_float(clamps, 9.999999f, maskAll);

    _pp_veq_int(maskDone, y, zeros, maskAll);

    while (_pp_cntbits(maskDone)<VECTOR_WIDTH) {
      __pp_mask masknotDone = _pp_mask_not(maskDone);
      _pp_vmult_float(result, result, x, masknotDone);
      _pp_vsub_int(y, y, ones, masknotDone);
      _pp_veq_int(maskDone, y, zeros, masknotDone);

      __pp_mask maskClamp;
      maskClamp = _pp_init_ones(0);
      _pp_vgt_float(maskClamp, result, clamps, masknotDone);
      _pp_vset_float(result, 9.999999f, maskClamp);
      maskDone = _pp_mask_or(maskDone, maskClamp);
    }


    _pp_vstore_float(output+i*VECTOR_WIDTH, result, maskAll);
  }


}




// returns the sum of all elements in values
// You can assume N is a multiple of VECTOR_WIDTH
// You can assume VECTOR_WIDTH is a power of 2
float arraySumVector(float* values, int N) {


  __pp_vec_float x;
  __pp_mask maskAll = _pp_init_ones();
  float sum = 0.0;
  float loops = log2(VECTOR_WIDTH);

  for (int i=0; i<N; i+=VECTOR_WIDTH) {
    _pp_vload_float(x, values+i*VECTOR_WIDTH, maskAll);

    int j=0;
    while(true) {
      _pp_hadd_float(x, x);
      ++j;
      if (!(j<loops))
	 break;
      _pp_interleave_float(x, x);
    }
    sum += x.value[0];
  }
  return sum;
}
