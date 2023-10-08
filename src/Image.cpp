#include "Image.h"
#include "../src/group_members.h"
#include <iostream>


Image::Image(){}
Image::Image(unsigned char* data, int w, int h, int c) : data(data), w(w), h(h), ch(c), size(w*h*ch){
    if(!data){
        std::cerr << "Image is empty" << std::endl;
        exit(-1);
    }
}
Image::~Image(){
}
