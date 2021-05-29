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

This program produces the image file mandelbrot-serial.ppm, which is a visualization of a famous set of complex numbers called the Mandelbrot set. [Most platforms have a .ppm viewer. For example, to view the resulting images, use tiv command (already installed) to display them on the terminal.]

As you can see in the images below, the result is a familiar and beautiful fractal. Each pixel in the image corresponds to a value in the complex plane, and the brightness of each pixel is proportional to the computational cost of determining whether the value is contained in the Mandelbrot set. To get image 2, use the command option --view 2. (See function mandelbrotSerial() defined in mandelbrotSerial.cpp). You can learn more about the definition of the Mandelbrot set.



## Hw3 [OpenMP Programming](https://nctu-sslab.github.io/PP-f20/HW3/)

## Hw4 [MPI Programming](https://nctu-sslab.github.io/PP-f20/HW4/)

## Hw5 [CUDA Programming](https://nctu-sslab.github.io/PP-f20/HW5/)

## Hw6 [OpenCL Programming](https://nctu-sslab.github.io/PP-f20/HW6/)
