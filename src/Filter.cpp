#include "Filter.h"
#include "Volume.h"
#include <iostream>
#include <cmath>
#include <vector>
#include "../lib/utility.h"
#include "../src/group_members.h"
#include "Projection.h"

Filter::Filter(Image im){
    this->data = im.data;
    this->w = im.w;
    this->h = im.h;
    this->ch = im.ch;
    this->size = im.size;
}

Filter::Filter(Volume vol) : data3D(vol.data3D){
    w = vol.w;
    h = vol.h;
    d = vol.d;
    ch = vol.ch;
    data = vol.flat;
    size = vol.size;
}

Filter::~Filter(){}


unsigned char* Filter::grayscale(unsigned char* input)
{
    // This function must pass image data which is RGB or RGBA format.
    // This function will return one grayscale channel image.

    unsigned char* dataset = this->data;
    unsigned char* output;
    int c = this->ch;
    if(c==1)
        return this->data;

    if(input)
        dataset = input;
        
    output = new unsigned char[this->w*this->h];
    
    // iterate over all pixels belonging to the RGB channels and merge them into a single grayscale channel
    for (int i = 0; i < w * h; i ++)
    {
        unsigned char r = dataset[c * i];
        unsigned char g = dataset[c * i + 1];
        unsigned char b = dataset[c * i + 2];
        // alpha channels do not need to be processed
        unsigned char gray = 0.21*r + 0.72*g + 0.07*b;
        
        output[i] = gray;
    }

    return output;
}

unsigned char* Filter::acb()
{
    // This function must pass image data which is RGB or RGBA format.
    // This function will return the same format as the input image data.
    unsigned char* dataset = this->data;
    int w = this->w;
    int h = this->h;
    int c = this->ch;
    if(c<3){
        std::cerr << "Function acb() only takes RGB or RGBA pictures" << endl;
        exit(-1);
    }


    int num_pix_chan = w * h;
    double sum_r = 0, sum_g = 0, sum_b = 0;
    unsigned char* output = new unsigned char[this->w*this->h*this->ch];
    
    // iterate over all pixels belonging to the RGB channels
    for (int i = 0; i < w * h; i ++)
    {
        sum_r += dataset[c * i];
        sum_g += dataset[c * i + 1];
        sum_b += dataset[c * i + 2];
    }
    
    double avg_r = sum_r / num_pix_chan;
    double avg_g = sum_g / num_pix_chan;
    double avg_b = sum_b / num_pix_chan;
    
    double avg_all = (avg_r + avg_g + avg_b) / 3;
    
    // calculate scaling constants
    double factor_r = avg_all / avg_r;
    double factor_g = avg_all / avg_g;
    double factor_b = avg_all / avg_b;
    
    // to prevent some pixel values from exceeding 255, an upper limit is therefore set
    for (int i = 0; i < w * h; i ++)
    {
        output[c * i] = dataset[c * i] * factor_r > 255 ? 255:dataset[c * i] * factor_r;
        output[c * i + 1] = dataset[c * i + 1] * factor_g > 255 ? 255:dataset[c * i + 1] * factor_g;
        output[c * i + 2] = dataset[c * i + 2] * factor_b > 255 ? 255:dataset[c * i + 2] * factor_b;
        if (c == 4) {output[c * i + 3] = dataset[c * i + 3];}
    }

    return output;
}

unsigned char* Filter::add_brightness(int brightness){

    unsigned char* output = new unsigned char[this->w*this->h*this->ch];
    unsigned char* dataset = this->data;

    if(this->ch == 4){
        for (int i=0; i<this->size; i++){
            if(i%4==3)
                output[i] = dataset[i];
            else if(brightness > 0)
                output[i] = util::minimum(dataset[i]+brightness, 255);
            else
                output[i] = util::maximum(dataset[i]+brightness, 0);
        }
    } else {
        for (int i=0; i<this->size; i++){
            if(brightness > 0)
                output[i] = util::minimum(dataset[i]+brightness, 255);
            else
                output[i] = util::maximum(dataset[i]+brightness, 0);
        }
    }

    return output;
}

unsigned char* Filter::average_brightness(){

    unsigned char* dataset = this->data;
    unsigned char* output;
    int w = this->w;
    int h = this->h;
    int ch = this->ch;

    int total = 0;
    if(ch == 4){
        for (int i = 0; i < this->size; i++){
            if(i%ch==3) // skip alpha
                continue;
            total += dataset[i]; // sum over all pixels
        }
    } else {
        for (int i = 0; i < this->size; i++){
            total += dataset[i]; // sum over all pixels
        }
    }

    int factor = 128 - total/(w*h*(ch-1)); // calculate difference

    output = add_brightness(factor);
    
    return output;
}

unsigned char* Filter::brightness(int brightness)
{
    unsigned char* output;

    // check if acceptable value
    if(brightness < -255 || brightness > 255)
        std::cerr << "Brightness value is not within acceptable bounds" << std::endl;

    if(brightness==0)
        output = average_brightness(); // make brightness an average of 28
    else
        output = add_brightness(brightness); // add brightness value to all pixels

    return output;
}

unsigned char* Filter::convolve(double** kernel, int kernel_size){
    int h = this->h;
    int w = this->w;
    int c = this->ch;
    int pad = (kernel_size - 1) / 2;
    int paddedwidth = w + 2 * pad;
    int paddedheight = h + 2 * pad;

    unsigned char* output = new unsigned char[w*h*c];

    // allocate the memory size of the paddeddata in the heap
    unsigned char* paddeddata = new unsigned char[paddedwidth * paddedheight * c]();
    // generate the padded data 1D array with applying replicate padding
    for (int y = 0; y < paddedheight; y++) {
        for (int x = 0; x < paddedwidth; x++) {
            for (int channel = 0; channel < c; channel++) {
                    int Y = util::maximum(0, util::minimum(y - pad, h - 1)); // replicated padding
                    int X = util::maximum(0, util::minimum(x - pad, w - 1));
                    paddeddata[(y * paddedwidth + x) * c + channel] = this->data[(Y * w + X) * c + channel];
            }
        }
    }

    // convolute using the filter
    double sum;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            for (int channel = 0; channel < c; channel++) {
                sum = 0; // reinitialise at each kernel
                for (int ky = 0; ky <= 2*pad; ky++) {
                    for (int kx = 0; kx <= 2*pad; kx++) {
                        int Y = y + ky;
                        int X = x + kx;
                        sum += paddeddata[(Y * paddedwidth + X) * c + channel]*kernel[ky][kx]; // weighted sum over kernel
                    }
                }
                output[(y * w + x) * c + channel] = (int) sum;
            }
        }
    }

    delete [] paddeddata;

    return output;
}


unsigned char* Filter::median_blur_2d(int kernel_size)
{
    int h = this->h;
    int w = this->w;
    int c = this->ch;
    int pad = (kernel_size - 1) / 2;
    int paddedwidth = w + 2 * pad;
    int paddedheight = h + 2 * pad;

    unsigned char* output = new unsigned char[w*h*c]();

    // allocate the memory size of the paddeddata in the heap
    unsigned char* paddeddata = new unsigned char[paddedwidth * paddedheight * c]();
    // generate the padded data 1D array with applying replicate padding
    for (int y = 0; y < paddedheight; y++) {
        for (int x = 0; x < paddedwidth; x++) {
            for (int channel = 0; channel < c; channel++) {
                    int Y = util::maximum(0, util::minimum(y - pad, h - 1)); // replicated padding
                    int X = util::maximum(0, util::minimum(x - pad, w - 1));
                    paddeddata[(y * paddedwidth + x) * c + channel] = this->data[(Y * w + X) * c + channel];
            }
        }
    }

    // convolute using the median filter
    int kernelvol = kernel_size*kernel_size;
    unsigned char values[kernelvol];
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            for (int channel = 0; channel < c; channel++) {
                for (int ky = 0; ky <= 2*pad; ky++) {
                    for (int kx = 0; kx <= 2*pad; kx++) {
                        int Y = y + ky;
                        int X = x + kx;
                        values[ky*kernel_size + kx] = paddeddata[(Y * paddedwidth + X) * c + channel];
                    }
                }
                output[(y * w + x) * c + channel] = util::get_median_odd(values, kernelvol);
            }
        }
    }

    delete [] paddeddata;

    return output;
}


double** Filter::gaussian_kernel(int n){
    double** kernel = new double*[n];
    for(int i=0;i<n;i++)
        kernel[i] = new double[n];

    double sigma = 1.0;
    int start = -(n/2);
    int end = -start;
    double sum = 0.0;
    double dist;

    for(int x=start;x!=end+1;x++)
        for(int y=start;y!=end+1;y++){
            dist = x * x + y * y;
            kernel[y-start][x-start] = 1/(2*M_PI*sigma*sigma) * exp(-dist/(2*sigma*sigma)); // implements gaussian function
            sum += kernel[y-start][x-start];
        }

    for(int i=0; i<n;i++)
        for(int j=0;j<n;j++)
            kernel[i][j] /= sum; // normalise

    return kernel;
}


unsigned char* Filter::gaussian_blur(int kernel_size)
{
    double** kernel = gaussian_kernel(kernel_size);

    unsigned char* output = convolve(kernel, kernel_size); // convolve kernel and image

    for(int i=0;i<kernel_size;i++)
        delete [] kernel[i];
    delete [] kernel;

    return output; 
}

unsigned char* Filter::boxblur(int kernelsize) {

    double** kernel = new double*[kernelsize];
    for(int i=0;i<kernelsize;i++)
        kernel[i] = new double[kernelsize];

    for(int x=0;x<kernelsize;x++)
        for(int y=0;y<kernelsize;y++){
            kernel[x][y] = 1.0/(kernelsize*kernelsize); // normalise
        }

    unsigned char* output = convolve(kernel, kernelsize); // convolve kernel with image

    for(int i=0;i<kernelsize;i++)
        delete [] kernel[i];
    delete [] kernel;

    return output;
}

unsigned char* Filter::histogram_equalize()
{
    int width = this->w;
    int heigth = this->h;
    unsigned char* gray_image = grayscale();
    unsigned char* output = new unsigned char[width*heigth];
    
    // Calculate the histogram of the image
    vector<int> histogram(GRAY_LEVELS, 0);
    for (int i = 0; i < width * heigth; i ++){
        histogram[gray_image[i]]++;
    }
    
    // Calculate the cumulative distribution function (CDF) of the normalized histogram
    vector<double> cdf (GRAY_LEVELS, 0.);
    cdf[0] = histogram[0]/double(width * heigth);
    for (int i = 1; i < GRAY_LEVELS; i++) {
        cdf[i] = cdf[i-1] + histogram[i]/double(width * heigth);
    }
    
    // Scale the CDF to the range [0, num_levels_-1], find the adjused grayscale.
    for (int i = 0; i < GRAY_LEVELS; i++) {
        cdf[i] = round(cdf[i] * (GRAY_LEVELS - 1));
    }
    
    // Apply histogram equalization to the image
    for (int i = 0; i < width * heigth; i++){
        output[i] = cdf[gray_image[i]];
    }

    return output;
}

unsigned char* Filter::apply_operator(int** horizontal, int** vertical, int factor)
{
    unsigned char* output = new unsigned char[this->w*this->h*this->ch];
    for(int i=0;i<this->w*this->h*this->ch;i++)
        output[i] = this->data[i];
    
    unsigned char* dataset;
    if(this->newimage){
        dataset = gaussian_blur(5); // if new image, put it through gaussian and possibly grayscale
        if(this->ch>1)
            dataset = grayscale(dataset);
    } else {
        dataset = this->data; // else, just apply to the last edge detection result
    }

    w = this->w;
    h = this->h;
    
    // since the kernel size is 3*3, for x and y, the range of pixels is [1, -2]
    for (int x = 1; x < w - factor; x++)
    {
        for (int y = 1; y < h - factor; y++)
        {
            int Gx = 0;
            int Gy = 0;
            // index pixels around
            for (int i = -1; i <= factor; i++)
            {
                for (int j = -1; j <= factor; j++)
                {
                    // apply operator to each pixel
                    int pixel = dataset[(y + i) * w + (x + j)];
                    Gx += pixel * horizontal[i + 1][j + 1];
                    Gy += pixel * vertical[i + 1][j + 1];
                }
            }
            unsigned char Gxy = sqrt(Gx * Gx + Gy * Gy);
            output[y * w + x] = Gxy;
        }
    }

    this->newimage = false;
    this->data = output;
    
    return output;
}

unsigned char* Filter::prewitt()
{
    int** horizontal = new int*[3];
    horizontal[0] = new int[3]{1, 0, -1};
    horizontal[1] = new int[3]{1, 0, -1};
    horizontal[2] = new int[3]{1, 0, -1};

    int** vertical = new int*[3];
    vertical[0] = new int[3]{1,1,1};
    vertical[1] = new int[3]{0,0,0};
    vertical[2] = new int[3]{-1,-1,-1};
    
    return apply_operator(horizontal, vertical, 1);
}

unsigned char* Filter::scharr()
{
    int** horizontal = new int*[3];
    horizontal[0] = new int[3]{3, 0, -3};
    horizontal[1] = new int[3]{10, 0, -10};
    horizontal[2] = new int[3]{3, 0, -3};

    int** vertical = new int*[3];
    vertical[0] = new int[3]{3,10,3};
    vertical[1] = new int[3]{0,0,0};
    vertical[2] = new int[3]{-3,-10,-3};

    
    return apply_operator(horizontal, vertical, 1);
}

unsigned char* Filter::sobel()
{
    int** horizontal = new int*[3];
    horizontal[0] = new int[3]{-1,0,1};
    horizontal[1] = new int[3]{-2,0,2};
    horizontal[2] = new int[3]{-1,0,1};

    int** vertical = new int*[3];
    vertical[0] = new int[3]{-1,-2,-1};
    vertical[1] = new int[3]{0,0,0};
    vertical[2] = new int[3]{1,2,1};
    
    return apply_operator(horizontal, vertical, 1);
}


unsigned char* Filter::robert()
{
    int** horizontal = new int*[2];
    horizontal[0] = new int[2]{1, 0};
    horizontal[1] = new int[2]{0,-1};

    int** vertical = new int*[2];
    vertical[0] = new int[2]{0, 1};
    vertical[1] = new int[2]{-1,0};
    
    return apply_operator(horizontal, vertical, 0);
}


double*** Filter::gaussian_kernel_3D(int n, double sigma){
    double*** kernel = new double**[n];
    for(int i=0;i<n;i++){
        kernel[i] = new double*[n];
        for(int j=0;j<n;j++)
            kernel[i][j] = new double[n];
    }

    int start = -(n/2);
    int end = -start;
    double sum = 0.0;
    double dist;

    for(int x=start;x!=end+1;x++)
        for(int y=start;y!=end+1;y++){
            for(int z=start;z!=end+1;z++){
                dist = x * x + y * y + z * z;
                kernel[z-start][y-start][x-start] = 1/(2*M_PI*sigma*sigma) * exp(-dist/(2*sigma*sigma)); // gaussian function
                sum += kernel[z-start][y-start][x-start];
            }
        }

    for(int i=0; i<n;i++)
        for(int j=0;j<n;j++)
            for(int k=0;k<n;k++)
                kernel[i][j][k] /= sum; // normalise

    return kernel;

}


unsigned char** Filter::convolve3D(double*** kernel, int kernel_size){
    int h = this->h;
    int w = this->w;
    int d = this->d;
    int c = this->ch;
    int pad = (kernel_size - 1) / 2;
    int paddedwidth = w + 2 * pad;
    int paddedheight = h + 2 * pad;
    int paddeddepth = d + 2 * pad;

    // allocate the memory size of the paddeddata in the heap
    unsigned char** paddeddata = new unsigned char*[paddeddepth];
    for(int i=0;i<paddeddepth;i++)
        paddeddata[i] = new unsigned char[paddedwidth * paddedheight * c]();

    unsigned char** output = new unsigned char*[paddeddepth];
    for(int i=0;i<paddeddepth;i++)
        output[i] = new unsigned char[paddedwidth * paddedheight * c]();

    // generate the padded data 1D array with applying replicate padding
    for (int z=0; z < paddeddepth; z++){
        for (int y =0; y < paddedheight; y++) {
            for (int x =0; x < paddedwidth; x++) {
                for (int channel = 0; channel < c; channel++) {
                    int Z = util::maximum(0, util::minimum(z - pad, d - 1)); // replicated padding
                    int Y = util::maximum(0, util::minimum(y - pad, h - 1));
                    int X = util::maximum(0, util::minimum(x - pad, w - 1));
                    paddeddata[z][(y * paddedwidth + x) * c + channel] = this->data3D[Z][(Y * w + X) * c + channel];
                }
            }
        }
    }

    // convolute using the box blur filter
    double sum;
    for (int z = 0; z < d; z++){
        //cout << "Convolving snapshot " << z+1 << endl;
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                // kernel
                for (int channel = 0; channel < c; channel++) {
                    sum = 0;
                    for(int kz = 0; kz <= 2*pad; kz++){
                        for (int ky = 0; ky <= 2*pad; ky++) {
                            for (int kx = 0; kx <= 2*pad; kx++) {
                                int Z = z + kz;
                                int Y = y + ky;
                                int X = x + kx;
                                sum += paddeddata[Z][(Y * paddedwidth + X) * c + channel]*kernel[kz][ky][kx]; // weighted average
                            }
                        }
                    }
                    output[z][(y * w + x) * c + channel] = (int) sum;
                }

            }
        }
    }

    for(int i=0;i<paddeddepth;i++)
        delete paddeddata[i];

    delete [] paddeddata;

    return output;
}

unsigned char** Filter::gaussian_blur_3D(int kernelsize){
    double*** kernel = gaussian_kernel_3D(kernelsize);

    unsigned char** output = convolve3D(kernel, kernelsize);

    return output; 
}


unsigned char** Filter::median_blur_3D(int kernel_size){
    int h = this->h;
    int w = this->w;
    int d = this->d;
    int c = this->ch;
    int pad = (kernel_size - 1) / 2;
    int paddedwidth = w + 2 * pad;
    int paddedheight = h + 2 * pad;
    int paddeddepth = d + 2 * pad;

    // allocate the memory size of the paddeddata in the heap
    unsigned char** paddeddata = new unsigned char*[paddeddepth];
    for(int i=0;i<paddeddepth;i++)
        paddeddata[i] = new unsigned char[paddedwidth * paddedheight * c]();

    unsigned char** output = new unsigned char*[paddeddepth];
    for(int i=0;i<paddeddepth;i++)
        output[i] = new unsigned char[paddedwidth * paddedheight * c]();

    // generate the padded data 1D array with applying replicate padding
    for (int z=0; z < paddeddepth; z++){
        for (int y =0; y < paddedheight; y++) {
            for (int x =0; x < paddedwidth; x++) {
                for (int channel = 0; channel < c; channel++) {
                    int Z = util::maximum(0, util::minimum(z - pad, d - 1));
                    int Y = util::maximum(0, util::minimum(y - pad, h - 1));
                    int X = util::maximum(0, util::minimum(x - pad, w - 1));
                    paddeddata[z][(y * paddedwidth + x) * c + channel] = this->data3D[Z][(Y * w + X) * c + channel];
                }
            }
        }
    }

    // convolute using the box blur filter
    int kernelvol = kernel_size*kernel_size*kernel_size;
    unsigned char values[kernelvol];
    for (int z = 0; z < d; z++){
        //cout << "Convolving snapshot " << z+1 << endl;
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                // kernel
                for (int channel = 0; channel < c; channel++) {
                    for(int kz = 0; kz <= 2*pad; kz++){
                        for (int ky = 0; ky <= 2*pad; ky++) {
                            for (int kx = 0; kx <= 2*pad; kx++) {
                                int Z = z + kz;
                                int Y = y + ky;
                                int X = x + kx;
                                values[kz*kernel_size*kernel_size + ky*kernel_size + kx] = paddeddata[Z][(Y * paddedwidth + X) * c + channel];
                            }
                        }
                    }
                    output[z][(y * w + x) * c + channel] = util::get_median_odd(values, kernelvol);
                }

            }
        }
    }

    for(int i=0;i<paddeddepth;i++)
        delete paddeddata[i];

    delete [] paddeddata;

    return output;
}
