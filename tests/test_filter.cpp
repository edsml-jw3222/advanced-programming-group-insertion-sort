#include <iostream>
#include <cassert>
#define STB_IMAGE_IMPLEMENTATION
#include "test_filter.h"
#include "../lib/stb_image.h"
#include "../src/Image.h"
#include "../src/Filter.h"
#include "../src/group_members.h"

void TestFilter::test_grayscale(){
    unsigned char* data = new unsigned char[12]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    double* solution = new double[4]{1.86, 4.86, 7.86, 10.86};

    Image im(data, 2, 2, 3);
    Filter filter(im);
    data = filter.grayscale();

    for(int i=0;i<4;i++)
    {
        assert(abs(data[i]-solution[i]) < 1);
    }

    delete[] data;
    delete[] solution;

    std::cout << "grayscale test passed" << endl;
}


void TestFilter::test_acb(){
    unsigned char* data = new unsigned char[12]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    double* solution = new double[12]{1.18, 2, 2.6, 4.73, 5, 5.2, 8.27, 8, 7.8, 11.82, 11, 10.4};

    Image im(data, 2, 2, 3);
    Filter filter(im);
    data = filter.acb();

    for(int i=0;i<12;i++)
    {
        assert(abs(data[i]-solution[i]) < 1);
    }

    delete[] data;
    delete[] solution;

    std::cout << "acb test passed" << endl;
}

void TestFilter::test_brightness(){
    int w, h, c;
    unsigned char* data = stbi_load("../Images/gracehopper.png", &w, &h, &c, 0);
    Image im(data, w, h, c);
    Filter filter(im);

    data = filter.brightness();

    int total = 0;
    for (int i = 0; i < im.size; i++){
        if(i%c==3)
            continue;
        total += data[i];
    }

    assert(total/(w*h*3) - 128 > -5);

    stbi_image_free(data);

    std::cout << "brightness test passed" << endl;
}

void TestFilter::test_median_blur_2d(){
    int w = 5, h = 5, c = 1;
    unsigned char* data = new unsigned char[25]{2,2,2,2,2,4,4,4,4,4,6,6,6,6,6,8,8,8,8,8,10,10,10,10,10};
    unsigned char* solution = new unsigned char[25]{2,2,2,2,2,4,4,4,4,4,6,6,6,6,6,8,8,8,8,8,10,10,10,10,10};

    Image im(data, w, h, 1);
    Filter filter(im);

    unsigned char* output = new unsigned char[w * h * c];
    output = filter.median_blur_2d(3);

    for (int i = 0; i < w * h * c; i++) {
        assert(abs(output[i] - solution[i]) < 1);
    }

    delete[] data;
    delete[] solution;
    delete[] output;

    std::cout << "2D median blur test passed" << std::endl;
}

void TestFilter::test_gaussian_blur(){
    int w, h, c;
    unsigned char* data = new unsigned char[25]{0,0,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,100};
    double* solution = new double[25]{0,0,0,0,0,0,7.5,12,7.5,0,0,12,20,12,0,0,7.5,12,15,19.5,0,0,0,19.5,51.5};

    // Read in image file
    Image im(data, 5, 5, 1);
    Filter filter(im);

    data = filter.gaussian_blur(3);

    for(int i=0;i<25;i++){
        assert(abs(data[i]-solution[i]) < 1);
    }

    delete[] data;
    delete[] solution;

    std::cout << "2D gaussian blur test passed" << std::endl;
}

void TestFilter::test_box_blur(){
    unsigned char* data = new unsigned char[25]{100,100,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,100};
    unsigned char* solution = new unsigned char[25]{52,40,28,16,4,36,28,20,12,4,20,16,16,16,16,4,4,12,20,28,4,4,16,28,40};

    // Read in image file    
    Image im(data, 5, 5, 1);
    Filter filter(im);
    unsigned char* result = filter.boxblur(5);
    for(int i=0;i<25;i++){
        assert(abs(result[i]-solution[i]) < 1);
    }
    
    delete[] data;
    delete[] solution;
    delete[] result;

    std::cout << "box_blur test passed" << std::endl;
}

void TestFilter::test_histogram(){
    const int width = 3;
    const int height = 3;

    unsigned char* data = new unsigned char[width*height]{255,210,50,100,100,50,210,50,210};
    Image im(data, width, height, 1);
    Filter filter(im);

    unsigned char* output = filter.histogram_equalize();

    unsigned char expected_output[width*height] = {255,227,84,140,140,84,227,84,227};

    double tolerance = 5; // how to decide this value?
    for (int i = 0; i < width*height; i++){
        assert(abs(output[i] - expected_output[i]) < tolerance);
    }

    delete[] data;
    delete[] output;

    std::cout << "histogram test passed!" << endl;
}

void TestFilter::test_robert(){
    unsigned char* input = new unsigned char[4]{1, 2, 3, 4};
    double* solution = new double[4]{1, 2, 3, 3.16};
    unsigned char* output;

    Image im(input, 2, 2, 1);
    Filter filter(im);
    filter.newimage = false; // skipping the gaussian blur and grayscale transformations
    output = filter.robert();

    for(int i=0;i<4;i++)
    {
        assert(abs(output[i]-solution[i]) < 1);
    }

    delete[] input;
    delete[] solution;
    delete[] output;

    std::cout << "robert test passed" << endl;
}

void TestFilter::test_sobel(){
    unsigned char* data = new unsigned char[9]{1, 2, 3, 4, 5, 6, 7, 8, 9};
    double* solution = new double[9]{1, 2, 3, 4, 25.3, 6, 7, 8, 9};

    Image im(data, 3, 3, 1);
    Filter filter(im);
    filter.newimage = false; // skipping the gaussian blur and grayscale transformations
    data = filter.sobel();

    for(int i=0;i<9;i++)
    {
        assert(abs(data[i]-solution[i]) < 1);
    }

    delete[] data;
    delete[] solution;

    std::cout << "sobel test passed" << endl;
}

void TestFilter::test_prewitt(){
    unsigned char* data = new unsigned char[9]{1, 2, 3, 4, 5, 6, 7, 8, 9};
    double* solution = new double[9]{1, 2, 3, 4, 18.97, 6, 7, 8, 9};

    Image im(data, 3, 3, 1);
    Filter filter(im);
    filter.newimage = false; // skipping the gaussian blur and grayscale transformations
    data = filter.prewitt();

    for(int i=0;i<9;i++)
    {
        assert(abs(data[i]-solution[i]) < 1);
    }

    delete[] data;
    delete[] solution;

    std::cout << "prewitt test passed" << endl;
}

void TestFilter::test_scharr(){
    unsigned char* data = new unsigned char[9]{1, 2, 3, 4, 5, 6, 7, 8, 9};
    double* solution = new double[9]{1, 2, 3, 4, 101.2, 6, 7, 8, 9};

    Image im(data, 3, 3, 1);
    Filter filter(im);
    filter.newimage = false; // skipping the gaussian blur and grayscale transformations
    data = filter.scharr();

    for(int i=0;i<9;i++)
    {
        assert(abs(data[i]-solution[i]) < 1);
    }

    delete[] data;
    delete[] solution;
    
    std::cout << "scharr test passed" << endl;
}


void TestFilter::test_median_blur_3d(){
    int w = 5, h = 5, d = 2, c = 1;
    unsigned char** data = new unsigned char*[2];
    for (int i = 0; i < d; i++) {
        data[i] = new unsigned char[25]{2,2,2,2,2,4,4,4,4,4,6,6,6,6,6,8,8,8,8,8,10,10,10,10,10};
    }
    unsigned char* solution = new unsigned char[w * h * c]{2,2,2,2,2,4,4,4,4,4,6,6,6,6,6,8,8,8,8,8,10,10,10,10,10};

    Volume vol(data, w, h, d, 1);
    Filter filter(vol);
    unsigned char** output;
    output = filter.median_blur_3D(3);

    for (int i = 0; i < w * h * c; i++) {
        assert(abs(output[0][i] - solution[i]) < 1);
    }

    for (int i = 0; i < d; i++) {
        delete[] data[i];
    }

    delete[] solution;
    delete[] output;

    std::cout << "3D median blur test passed" << std::endl;
}


void TestFilter::test_gaussian_blur_3d(){
    int w = 5, h = 5, d = 2, c = 1;
    unsigned char** data = new unsigned char*[2];

    data[0] = new unsigned char[25]{100,0,0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0};
    data[1] = new unsigned char[25]();
    
    unsigned char* solution = new unsigned char[w * h * c]{13,5,0,0,0,5,4,3,2,0,0,3,5,3,0,0,2,3,2,0,0,0,0,0,0};

    Volume vol(data, w, h, d, 1);
    Filter filter(vol);
    unsigned char** output;
    output = filter.gaussian_blur_3D(3);

    for (int i = 0; i < w * h * c; i++) {
        assert(abs(output[1][i] - solution[i]) < 1.5);
    }

    for (int i = 0; i < d; i++) {
        delete[] data[i];
    }

    delete[] solution;
    delete[] output;

    std::cout << "3D gaussian blur test passed" << std::endl;
}
