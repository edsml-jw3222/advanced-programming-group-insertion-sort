#include "Projection.h"
#include <vector>
#include "../lib/utility.h"
#include "../src/group_members.h"
#include <iostream>

// the constructor to initialize the reference to the Volume object
Projection::Projection(const Volume& vol) {
    flat = vol.flat;
    data3D = vol.data3D;
    w = vol.w;
    h = vol.h;
    d = vol.d;
    ch = vol.ch;
}

Projection::~Projection(){}

// the function used for operating over a thin slab
void Projection::Slab(int& minZ, int& maxZ) {
    minZ = util::maximum(minZ, 0);
    maxZ = (maxZ == -1) ? this->d - 1 : util::minimum(maxZ, this->d - 1);
}

// Maximum Intensity Projection
unsigned char* Projection::mip(int minZ, int maxZ){
    Slab(minZ, maxZ);

    // allocate memory for pixels with 2D image size in heap
    size = this->w * this->h * this->ch;
    unsigned char* output = new unsigned char[size]();

    // iterate to find the max intensity value for every pixel position
    for (int i = 0; i < size; i++) {
        for (int d = minZ; d <= maxZ; d++) {
            if(this->data3D[d][i] > output[i])
                output[i] = this->data3D[d][i];
        }
    }
    return output;
}

// Minimum Intensity Projection
unsigned char* Projection::minip(int minZ, int maxZ){
    Slab(minZ, maxZ);

    size = this->w * this->h * this->ch;
    unsigned char* output = new unsigned char[size];

    // iterate to find the min intensity value for every pixel position
    for (int i = 0; i < size; i++) {
        output[i] = 255;
        for (int d = minZ; d <= maxZ; d++) {
            if(this->data3D[d][i] < output[i])
                output[i] = this->data3D[d][i];
        }
    }
    return output;
}


// Average Intensity Projection based on mean or median
unsigned char* Projection::aip(bool use_median, int minZ, int maxZ){
    Slab(minZ, maxZ);

    size = this->w * this->h * this->ch;
    unsigned char* output = new unsigned char[size];
    unsigned char** dataset = this->data3D;
   
    int numslices = maxZ - minZ + 1;

    unsigned char needle[this->d];
    if (use_median) { // compute the median value for every pixel position
        for (int i = 0; i < size; i++) {
            for (int d = minZ; d <= maxZ; d++) {
                needle[d] = dataset[d][i];
            }

            output[i] = util::get_median(needle, this->d);
        }
    } 
    
    else { // compute the mean value for every pixel position
        std::vector<unsigned int> Sum(size, 0);

        for (int d = minZ; d <= maxZ; d++) {
            for (int i = 0; i < size; i++) {
        // convert type from unsigned char(range:0-255) to unsigned int, then add
                Sum[i] += static_cast<unsigned int>(dataset[d][i]);
            }
        }

        // iterate to calculate the average value for every pixel position
        for (int i = 0; i < size; i++) {
            output[i] = static_cast<unsigned char>(Sum[i] / numslices);
        }
    }

    return output;
}

