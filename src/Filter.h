#pragma once
#include "Image.h"
#include "Volume.h"
#include "../src/group_members.h"
#include <vector>

using namespace std;

const int GRAY_LEVELS = 256;

/**
 * @class Filter
 * @brief A class for applying various 2D and 3D filters to Image and Volume objects.
 * 
 * The Filter class is derived from Image class and provides functionality to apply various 2D and 3D filters to Image and Volume objects. 
 */
class Filter : public Image
{
public:

    /**
     * Constructor for creating a Filter object from an Image object.
     * @param im The input Image object.
     */
    Filter(Image im);

    /**
     * Constructor for creating a Filter object from a Volume object.
     * @param vol The input Volume object.
     */
    Filter(Volume vol);
    ~Filter();

    // 2D Filters
    
    /**
     * @brief Converts an RGB or RGBA image to grayscale.
     * @return Pointer to the output grayscale image data. The size of the output array is w * h.
     */
    unsigned char* grayscale(unsigned char* input=nullptr);

    
    /**
     * @brief Adjusts each RGB channel so the average intensity of each channel is equal.
     * @return Pointer to the output balanced image data. The size of the output array is w * h * c.
     */
    unsigned char* acb();

    /**
     * @brief Adds brightness to the image.
     * @param brightness The integer value to be added to the pixel values of the image.
     * @return Pointer to the modified image data.
     */
    unsigned char* add_brightness(int brightness);

    /**
     * @brief Averages the brightness of the image.
     * @return Pointer to the modified image data with average brightness.
     */  
    unsigned char* average_brightness();

    /**
     * @brief Adjusts the brightness of the image.
     * @param brightness The integer value to adjust the brightness, 0 for automatic adjustment.
     * @return Pointer to the modified image data with adjusted brightness.
     */
    unsigned char* brightness(int brightness=0);

    /**
     * @brief  Performs histogram equalization on the image to improve contrast.
     * @return Pointer to the modified image data with improved contrast.
     */
    unsigned char* histogram_equalize();


    // convolutions

    /**
     * @brief Convolves the image with a given kernel.
     * @param kernel 2D array representing the kernel for convolution.
     * @param kernel_size Size of the kernel.
     * @return Pointer to the modified image data after convolution.
     */
    unsigned char* convolve(double** kernel, int kernel_size);


    
    /**
     * @brief Applies a 2D median filter to the input image.
     * @param kernel_size Size of the kernel, should be odd.
     * @return An unsigned char pointer to the filtered image.
     */
    unsigned char* median_blur_2d(int kernel_size);

    /**
     * @brief Performs box blur on the image.
     * @param kernel_size Size of the kernel for box blur.
     * @return Pointer to the modified image data after box blur.
     */
    unsigned char* boxblur(int kernel_size);

    /**
     * @brief Generates a Gaussian kernel.
     * @param n Size of the Gaussian kernel.
     * @return 2D array representing the Gaussian kernel.
     */
    double** gaussian_kernel(int n);

    /**
     * @brief Performs Gaussian blur on the image.
     * @param kernel_size Size of the kernel for Gaussian blur.
     * @return Pointer to the modified image data after Gaussian blur.
     */
    unsigned char* gaussian_blur(int kernel_size);


    // edge detection
    
    /**
     * @brief Applies convolution algorithm to a grayscale image.
     * @param horizontal[3][3] Kernel used to calculate the gradient in the x-direction.
     * @param vertical[3][3] Kernel used to calculate the gradient in the y-direction.
     * @return Pointer to the output image data. The size of the output array is w * h.
     */
    unsigned char* apply_operator(int** horizontal, int** vertical, int factor);
    
    
    /**
     * @brief Edge detection of input image using the Scharr operator.
     * @return Pointer to the output image data. The size of the output array is w * h.
     */
    unsigned char* scharr();
    
    /**
     * @brief Edge detection of input image using the Roberts' Cross operator.
     * @return Pointer to the output image data. The size of the output array is w * h.
     */
    unsigned char* robert();
    
    /**
     * @brief Edge detection of input image using the Prewitt operator.
     * @return Pointer to the output image data. The size of the output array is w * h.
     */
    unsigned char* prewitt();
    
    /**
     * @brief Edge detection of input image using the Sobel operator.
     * @return Pointer to the output image data. The size of the output array is w * h.
     */
    unsigned char* sobel();


    // 3D Filters

    /**
     * @brief Applies a 3D median filter to the input 3D volume.
     * @param kernel_size An integer specifying the size of the kernel. The kernel size must be an odd number.
     * @return A 3D unsigned char array of the same size as the input with the median filter applied.
     */
    unsigned char** median_blur_3D(int kernel_size);

    /**
     * @brief Convolves the 3D data with a given kernel.
     * @param kernel 3D array representing the kernel for convolution.
     * @param kernel_size Size of the kernel.
     * @return Pointer to the modified 3D data after convolution.
     */
    unsigned char** convolve3D(double*** kernel, int kernel_size);

    /**
     * @brief Generates a 3D Gaussian kernel.
     * @param n Size of the Gaussian kernel
     * @return 3D array representing the Gaussian kernel.
     */
    double*** gaussian_kernel_3D(int n, double sigma=1.0);

    /**
     * @brief Performs Gaussian blur on the 3D data.
     * @param kernelsize Size of the kernel for Gaussian blur.
     * @return Pointer to the modified 3D data after Gaussian blur.
     */
    unsigned char** gaussian_blur_3D(int kernelsize);

    unsigned char** data3D;
    int d;
    bool newimage = true;
};


