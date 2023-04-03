# Alpha Blending
## Project Descrtiption 

This project is a C++ program that utilizes the SFML library for image processing. Specifically, it implements alpha blending of two images using SIMD AVX instructions for improved performance.

![Picture](img/avx.png)

## Advantages of using SIMD AVX
SIMD (Single Instruction, Multiple Data) is a technique that allows a single instruction to be executed on multiple pieces of data at the same time, thus improving performance. AVX (Advanced Vector Extensions) is a specific implementation of SIMD that allows for 256-bit vectors to be processed at once.

In our program, we use AVX instructions to perform alpha blending of two images. The AVX instructions allow us to perform the same blending operation on multiple pixels at the same time, resulting in a significant speedup compared to non-SIMD implementations.

While compiler optimization flags such as -Ofast and -O3 can improve program performance, they may not always be able to take advantage of specific hardware features like SIMD. In contrast, using SIMD AVX instructions allows for more precise control over how data is processed, resulting in potentially greater performance gains.

Additionally, compiler flags can sometimes result in unintended consequences, such as changes to program behavior or increased memory usage. By using explicit SIMD instructions, we can ensure that our program behaves exactly as intended.


## Dependencies

This program requires SFML to be installed on your system. You can download SFML from the official website: https://www.sfml-dev.org/download.php

## Build and run

First of all, download this repository:
```bash
git@github.com:mr-gukas/MyALphaBlending.git
cd MyALphaBlending
```
Then you can choose the branch you want:
```bash
git checkout avx    \\ for version with AVX2
git checkout no_avx \\ for version without AVX2
```
Compile the program using CMake: 
```bash
mkdir build 
cd build 
cmake ..
make
```
Run the program:
```bash 
./alpha <front_path> <back_path> <x_pos> <y_pos> 
```

You can also choose on of compiler optimization flags. Write them in "CMakeLists.txt". For example::
```bash
add_compile_options(-Ofast)
```
## Results

**FPS rate:**

| Optimization key | No optimizations | AVX2 float  | 
|------------------|------------------|-------------|
|       None       |        6         |      25     |
|       -Ofast     |        15        |      94     |

## Conclusion

By utilizing SIMD AVX instructions in our image processing program, we were able to achieve a significant improvement in performance compared to non-SIMD implementations. This approach offers advantages over relying solely on compiler optimization flags, including greater control over program behavior and potential for greater performance gains.
