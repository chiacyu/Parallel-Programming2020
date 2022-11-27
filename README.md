# Parallel-Programming2020

## Intro
Those file are programming assignment of Parallel Programming in NCTU. The content are mostly referenced from [cs149](http://cs149.stanford.edu/fall20).
The total six project will focus on different parellel techniques which we would discuss later. The course wbesite would be included in the following subsection.



## Hw1 [Vectorizing Code Using Fake SIMD Intrinsics](https://nctu-sslab.github.io/PP-f20/HW1/)

### Vectorizing Code Using Fake SIMD Intrinsics

- [Report](https://hackmd.io/@4Hw858MgTO-qILMEkncvEA/B1wuQQhvD)

Take a look at the function clampedExpSerial in part1/main.cpp of the Assignment I code base. The clampedExp() function raises values[i] to the power given by exponents[i] for all elements of the input array and clamps the resulting values at 9.999999. Your job is to vectorize this piece of code so it can be run on a machine with SIMD vector instructions.

The output of the program will tell you if your implementation generates correct output. If there are incorrect results, the program will print the first one it finds and print out a table of function inputs and outputs. Your function’s output is after “output = “, which should match with the results after “gold = “. The program also prints out a list of statistics describing utilization of the PP fake vector units. You should consider the performance of your implementation to be the value “Total Vector Instructions”. (You can assume every PP fake vector instruction takes one cycle on the PP fake SIMD CPU.) “Vector Utilization” shows the percentage of vector lanes that are enabled.


## Hw2 [Multi-thread Programming](https://nctu-sslab.github.io/PP-f20//HW2/)

- [Report](https://hackmd.io/Ujutg1YrSW2ROCDbvkTz-w)

This program produces the image file mandelbrot-serial.ppm, which is a visualization of a famous set of complex numbers called the Mandelbrot set. [Most platforms have a .ppm viewer. For example, to view the resulting images, use tiv command (already installed) to display them on the terminal.]
As you can see in the images below, the result is a familiar and beautiful fractal. Each pixel in the image corresponds to a value in the complex plane, and the brightness of each pixel is proportional to the computational cost of determining whether the value is contained in the Mandelbrot set. To get image 2, use the command option --view 2. (See function mandelbrotSerial() defined in mandelbrotSerial.cpp). You can learn more about the definition of the Mandelbrot set.

![Medabrot](https://camo.githubusercontent.com/80f2e33b4e20f3f86809c6203402dc6807b389bc/687474703a2f2f67726170686963732e7374616e666f72642e6564752f636f75727365732f6373333438762d31382d77696e7465722f617373745f696d616765732f61737374312f6d616e64656c62726f745f76697a2e6a7067)



## Hw3 [OpenMP Programming](https://nctu-sslab.github.io/PP-f20/HW3/)


In this assignment, you are asked to parallelize a serial implementation of the conjugate gradient method using OpenMP. The serial implementation is in cg.c and cg_impl.c. Please refer to the README file for more information about the code.

In order to parallelize the conjugate gradient method, you may want to use profiling tools to help you explain the performance difference before and after your modification. As it probably just works ineffectively if you simply add a parallel-for directive for each for loop, you may want to use profiling tools, such as perf, time, gprof, Valgrind (not limited to these tools), to profile your program to better understand how much the improvement is from the code snippets you have modified.

![PageRank](https://user-images.githubusercontent.com/18013815/95656789-47c8fa00-0b43-11eb-80f5-56b7a849a831.png)

## Hw4 [MPI Programming](https://nctu-sslab.github.io/PP-f20/HW4/)

- [Report](https://hackmd.io/nv1e8F_SQSS064Oh8mCbSQ)

Here we’re going to implement our first MPI program. Expected knowledge includes basic understanding of the MPI environment, how to compile an MPI program, how to set the number of MPI processes, and retrieve the rank of the process and number of MPI processes at runtime. Let’s learn how to launch an MPI code on NCTU-PP workstations. There is a starter code for you. Look at hello.cc. Please finish the _TODO_s with the MPI_Comm_size and MPI_Comm_rank functions. We will work on NFS, so we don’t need to copy and compile the code again and again on different nodes.



## Hw5 [CUDA Programming](https://nctu-sslab.github.io/PP-f20/HW5/)


Following part 2 of HW2, we are going to parallelize fractal generation by using CUDA.
Build and run the code in the HW5 directory of the code base. (Type make to build, and ./mandelbrot to run it. ./mandelbrot --help displays the usage information.)
Your job is to parallelize the computation of the images using CUDA. A starter code that spawns CUDA threads is provided in function hostFE(), which is located in kernel.cu. This function is the host front-end function that allocates the memory and launches a GPU kernel. Currently hostFE() does not do any computation and returns immediately. You should add code to hostFE() function and finish mandelKernel() to accomplish this task. The kernel will be implemented, of course, based on mandel() in mandelbrotSerial.cpp, which is shown below. You may want to customized it for your kernel implementation.


## Hw6 [OpenCL Programming](https://nctu-sslab.github.io/PP-f20/HW6/)


Convolution is a common operation in image processing. It is used for blurring, sharpening, embossing, edge detection, and more. The image convolution process is accomplished by doing a convolution between a small matrix (which is called a filter kernel in image processing) and an image. You may learn more about the convolution process at Wikipedia: Convolution.

Figure 1 shows an illustration of the concept of applying a convolution filter to a specific pixel, value of which is 3. After the convolution process, the value of the pixel becomes 7—how the resulting value is computed is illustrated on the right of the figure.

![Convolution](https://user-images.githubusercontent.com/18013815/101978949-f7c9f900-3c93-11eb-8cc8-9b99db425b01.png)
