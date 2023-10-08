#include "Slice.h"
#include <iostream>
#include "../lib/stb_image_write.h"
#include "../src/group_members.h"

Slice::Slice(Volume& vol) : volume(vol) {
    this->data3D = volume.data3D;
    this->flat = volume.flat;
    this->w = volume.w;
    this->h = volume.h;
    this->d = volume.d;
    this->ch = volume.ch;
}

Slice::~Slice(){
    delete [] flat;
}

unsigned char* Slice::slice(int p0, char fixed_coor){
    int width = this->w;
    int height = this->h;
    int deep = this->d;
    int channels = this->ch;
    this->flat = this->volume.flatten_data();
    
    unsigned char* slice_plane;
    
    if (fixed_coor == 'x')
    {       
        slice_plane = new unsigned char [height*deep]();
        for (int i = 0; i < height * deep; i++){
            slice_plane[i] = this->flat[i*width+p0];
        }
    }
    else
    {       
        slice_plane = new unsigned char [width*deep]();
        for (int i = 0; i < deep; i++){
            for (int j = 0; j < width; j++){
                slice_plane[i*width+j] = this->flat[width*p0 + width*height*i + j];
            }
        }
    }
    
    return slice_plane;
}
        
