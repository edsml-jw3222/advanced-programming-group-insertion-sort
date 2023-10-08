#include "Volume.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include "../lib/stb_image.h"
#include "../lib/stb_image_write.h"
#include "../lib/utility.h"
#include "../src/group_members.h"


using std::cin; using std::cout;
using std::vector;
using std::filesystem::directory_iterator;
using std::endl; using std::string;


Volume::Volume(string dir){

    if (!std::filesystem::is_directory(dir) || !std::filesystem::exists(dir)){ // Check if scan folder exists
        cerr << "Scan directory does not exist" << endl;
        exit(-1);
    }

    vector<string> files;
    for (const auto & file: directory_iterator(dir)) {
        files = util::insert(files, file.path());
    }

    int w, h, c;

    this->data3D = new unsigned char*[files.size()];
    cout << "There are " << files.size() << " slices." << endl;

    for (int i=0;i<files.size();i++) {
        this->data3D[i] = stbi_load(files[i].c_str(), &w, &h, &c, 0);
    }

    this->w = w;
    this->ch = c;
    this->h = h;
    this->size = w*h*ch;
    this->d = files.size();
}

Volume::Volume(unsigned char** input, int w, int h, int d, int c){
    if(!input){
        std::cerr << "Volume double pointer is empty" << std::endl;
        exit(-1);
    }
    this->data3D = input;
    this->w = w;
    this->ch = c;
    this->h = h;
    this->size = w*h*c;
    this->d = d;
}

void Volume::operator()(unsigned char** init){
    if(this->data3D)
        this->data3D = init;
    else{
        std::cerr << "You can't initialise a volume the first time with only a data array" << std::endl;
        exit(-1);
    }
}

Volume::~Volume(){}

unsigned char* Volume::flatten_data(){
    unsigned char* slice;

    this->flat = new unsigned char[this->size*this->d];

    for(int i=0;i<this->d;i++){
        slice = this->data3D[i];
        for(int j=0;j<this->size;j++){
            this->flat[this->size*i + j] = slice[j];
        }
    }

    return this->flat;
}