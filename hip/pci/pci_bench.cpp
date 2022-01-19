#include <hip/hip_runtime.h>
#include <time.h>
#include <stdio.h>

double host_to_device( int N, int nReps ){

  float *f_cpu;
  float *f_gpu;
  clock_t t;
  double wallTime;

  // Allocate the host array
  f_cpu = (float*)malloc( N*sizeof(float) );

  // Allocate the device array
  hipMalloc(&f_gpu, N*sizeof(float));

  t = clock();
  for( int i=0; i < nReps; i++ ){
    hipMemcpy(f_gpu,f_cpu,N*sizeof(float), hipMemcpyHostToDevice);
  }
  hipDeviceSynchronize();
  t = clock() - t;
  
  wallTime = ((double)t)/CLOCKS_PER_SEC/nReps;

  free(f_cpu);
  hipFree(f_gpu);

  return wallTime;
}

int main ( ){

  int nMax = 100000000;
  int nStep = 10;

  printf("Size (Bytes), Wall Time (s), Bandwidth (GB/s) \n" );
  for( int i = 1; i<nMax; i *= nStep ){

    double wallTime = host_to_device( i, 1000 );
    int dataSize=sizeof(float)*i;
    double bandwidth=dataSize/wallTime/1024/1024/1024;

    printf("%d, %f, %f \n",dataSize,wallTime,bandwidth);
  } 

}
