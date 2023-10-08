# Image Filters, Projections and Slices Program

Made by Group Insertion-sort

Group Members: Jiajia Zheng, Jie Wang, Michael Tsesmelis, Shichao Hu, Suhan Zhao

## Contents

1. [Installation Guide](#installation-guide)
2. [Description of the Tool](#description-of-the-tool)
3. [User Instructions](#user-instructions)
4. [Testing](#testing)
5. [Results](#results)


## Installation Guide

You can download the package simply from Github. The entire package includes an image folder(Image), a library folder(lib), 
a source file folder(src), a test folder(tests), an example folder(examples) and some other initialisation files.

If you have never used C/C++ before, you can check out the following links to download it.

### For Windows User:

[```Windows```]
(https://learn.microsoft.com/en-us/cpp/build/walkthrough-compiling-a-native-cpp-program-on-the-command-line?view=msvc-170)

### For Mac User:

[```MacOS```]
(https://macosx-faq.com/how-to-compile-cpp-terminal/)

### For Linux User:

Linux users are likely to find C compilers either installed by default on their desktop distributions, or readily installable via the system package manager.

## Description of the Tool

Our library is a tool to apply image filters, orthographic projections and slices to images and 3D scans. More detailed descriptions can be found on our interactive web documentation. Simply go under the html folder, locate the index.html file and click on it.

### 2D Image Filters

For 2D image filters, we mainly have three different functions, namely colour correction, image blur and edge detection.
For each part we have 3 or 4 different methods as listed below.

1. Color Correction

   a) Grayscale
   
   The grayscale equation combines the pixels of the three R, G and B channels into a single greyscale channel.

   b) Automatic Colour balance
   
   Scale the pixels of each channel to achieve an equal average of the pixel values of the three R, G and B channels.

   c) Brightness
   
   Take an optional brightness value in the range -255 – 255 to add to all pixels, or perform an 
   automatic brightness filter by setting the average value of all pixels in all channels to 128.

   d) Histogram equaliztion
   
   Improve the contrast of an image by modifying the image's histogram.
   

2. Image blur

   a) Median blur
   
   Applies a convolution operation to reduce noise and blur in an image by replacing each pixel's value with the median value of its neighboring pixels 
   with a chosen odd kernel size.

   b) Box blur
   
   Applies a convolution operation to blur an image by averaging the pixel values in an arbitary odd kernel size centered 
   around each pixel.

   c) Gaussian blur
   
    Applies a convolution operation to reduce noise and blur in an image by convolving it with a Gaussian function.

3. Edge Detection

   a) Sobel
   
   Detecting edges in an image by calculating gradients using two 3x3 kernels.

   b) Prewitt
   
   Detecting edges in an image by calculating gradients using two 3x3 kernels.

   c) Scharr
   
   Detecting edges in an image by calculating gradients using two 3x3 kernels.
   
   d) Roberts' Cross
   
   Detecting edges in an image by calculating gradients using two 2x2 kernels.
   
  ### 3D Data Volume Filters, Projections, and Slices

   For 3D data volumes, we also mainly have three different functions, namely 3D filters, orthographic projection and Slicing.
   For each part we have 2 or 3 different methods as listed below.
   
1. 3D filters
   
   a) 3D Gaussian
   
   To replace the centre voxel with the weighted average of all the voxel intensities in the neighbourhood, with an arbitrary filter kernel size. Similar to 2D Gaussian filter, but with a 3D kernel.

   b) 3D Median
   
   To replace the centre voxel with the median of all the voxel intensities in the neighbourhood, with an arbitrary filter kernel size. Similar to 2D Median filter, but with a 3D kernel.
   
2. Orthographic Projections
   
   a) Maximum intensity projection (MIP) 
   
   Get the maximum intensity of all pixels with a given (x, y) coordinate in all images in the z-direction, and output that maximum value on the final image.

   b) Minimum intensity projection (MinIP)
   
   Similar to the MIP, but for the minimum intensity at each (x, y) coordinate.
   
   c) Average intensity projection (AIP)
   
   Similar to the MIP, but for the average (mean or median) intensity of all pixels with a given (x, y) location.
   
3. Slicing

   Slice the 3D volume in a different plane. For example, if all images are given in the x-y plane, we can output an image in the x-z or y-z plane (for a given y or x coordinate, respectively).
   

## User Instructions

1. Open the terminal and navigate to the program directory, which is named as ‘advanced-programming-group-insertion-sort’ by default. Then navigate to the folder ``src/``:
```
cd src
```

2. We already prepared the executable file ``main.o`` in ``src`` folder, type in the terminal to run:

```
./main.o
```

3. For Mac users, if you encounter the alert "permission denied" and cannot open the executable file, type the following command before `./main.o`:
```
chmod +x main.o
xattr -cr main.o
```

4. Follow the on-screen instructions to use the tool.


Note: If you want to compile all files to obtain a new executable file, type the command:
```
clang++ -std=c++17 src/main.cpp src/Filter.cpp src/Projection.cpp src/Slice.cpp src/Image.cpp src/Volume.cpp lib/utility.cpp -o src/main
```
or
```
g++ -std=c++17 src/main.cpp src/Filter.cpp src/Projection.cpp src/Slice.cpp src/Image.cpp src/Volume.cpp lib/utility.cpp -o src/main
```
then run executable:
```
./main
```

### Examples

Here is an example of using our tool to obtain a desired image after run the command:
```
./main
```
1. Choose path of image you want to operate on and specify a full path to save image:

<img src="https://user-images.githubusercontent.com/114110792/227522549-02544079-defd-46d5-889f-46dcd632d45b.png" width='80%' height='80%' align=center />

2. Choose the filters you want, and they will automatically appear in your output folder under the working directory ``src/``. For edge detection cases, since you can apply them sequentially, you must enter '5' if you want to save the image to the specified folder:

 <img src="https://user-images.githubusercontent.com/114110792/227523205-8a7a3c8f-6674-4dad-8563-8cbde860e13c.png" width='50%' height='50%' align=center /> <img src="https://user-images.githubusercontent.com/114110792/227523237-26f2d9a1-2b09-4318-9a4b-d8a1e6b0c4ee.png" width='40%' height='40%' align=center />

3. Choose to apply another filter or enter other numbers to finish current transformation:

 <img src="https://user-images.githubusercontent.com/114110792/227524191-953e14f7-f3e4-419f-b9d6-4cbf89dfadbc.png" width='40%' height='40%' align=center />

4. Then you can open your specified folder to see the resulting image of your transformation (`src/<user_specified_name>/` in this example):

<img src="https://user-images.githubusercontent.com/114110792/227558315-a063618d-9012-474f-ae27-bdcc790a06ec.png" width='60%' height='60%' align=center />


5. To exit the program, press `control + q`.


## Testing

If you want to see the results of our unit tests for all implemented filters, following the instructions:

1. Navigate to the program directory: ‘advanced-programming-group-insertion-sort’(by default).

2. We already prepared the executable file `unit_test.o` in `tests/` folder, type in the terminal to run:

```
cd tests
chmod +x unit_test.o
xattr -cr unit_test.o
./unit_test.o
```

2. You can compile the unit_test using following command:
```
clang++ -std=c++17 tests/test_filter.cpp tests/test_projections.cpp tests/test_slicing.cpp tests/unit_test.cpp src/Filter.cpp src/Projection.cpp src/Slice.cpp src/Image.cpp src/Volume.cpp lib/utility.cpp -o tests/unit_test
```
OR
```
g++ -std=c++17 tests/test_filter.cpp tests/test_projections.cpp tests/test_slicing.cpp tests/unit_test.cpp src/Filter.cpp src/Projection.cpp src/Slice.cpp src/Image.cpp src/Volume.cpp lib/utility.cpp -o tests/unit_test
```

3. Then navigate to the `tests/` folder:
```
cd tests
```

4. Type in the terminal to run the executable:

```
./unit_test
```


## Results

We have results for running all filters in our tool, which can be seen under the `Output/` folder. There are some examples after transforming the following images:

<img src="https://user-images.githubusercontent.com/114110792/227525727-db4f2292-5483-41d9-85a8-feab20b17d18.png" width='30%' height='30%' align=center />


1. Grayscale

<img src="https://user-images.githubusercontent.com/114110792/227525803-8dc2bff9-a439-43ed-9b43-c6f3cf0c34a4.png" width='30%' height='30%' align=center />


2. Automatic colour balance

<img src="https://user-images.githubusercontent.com/114110792/227525970-01ba1e38-02e5-4c41-8d81-5459bee3c12d.png" width='30%' height='30%' align=center />


3. Median blur

<img src="https://user-images.githubusercontent.com/114110792/227525878-344af280-453b-4970-89a1-1bd19f83e301.png" width='30%' height='30%' align=center />

4. Prewitt

<img src="https://user-images.githubusercontent.com/114110792/227526134-de4a37f3-8dbc-4ee5-9b72-330f5180806c.png" width='30%' height='30%' align=center />

5. XZ-slice

<img src="https://user-images.githubusercontent.com/114110792/227526224-d9acced3-066b-40d0-b628-2c22591f85ed.png" width='30%' height='30%' align=center />
   
