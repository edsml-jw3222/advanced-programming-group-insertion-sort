/*
Implements all images as requested by Prof. Davison

*/

#include <iostream>
#include <string>
#include <filesystem>
#include <chrono>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../lib/stb_image_write.h"
#include "../src/Image.h"
#include "../src/Filter.h"
#include "../src/Volume.h"
#include "../src/Projection.h"
#include "../src/Slice.h"
#include "../src/group_members.h"


string target_location(string filename, string fullname){
    /* Creates the destination folder and file for a given input file.
    */

    string name;
    string dest;

    // retrieve name without extension
    dest.append("../Output/");
    int found = filename.find_last_of("/");
    filename = filename.substr(found+1);
    int foundend = filename.find_last_of(".");
    name = filename.substr(0, foundend);
    dest.append(name);

    if (!std::filesystem::is_directory(dest) || !std::filesystem::exists(dest)) { // Check if src folder exists
        std::filesystem::create_directory(dest); // create src folder
    }

    dest.append("/");
    dest.append(fullname);

    cout << dest << endl;

    return dest;
}

void apply_filters(const string filename){
    /*
    Applies 2D filters
    */

    int w, h, c;
    unsigned char* data;
    unsigned char* image;
    string location;
    int success;

    auto start = chrono::steady_clock::now();

    // Read in image file
    image = stbi_load(filename.c_str(), &w, &h, &c, 0);
    cout << filename << endl;
    unsigned char* grayscale;
    Image im(image, w, h, c);
    Filter filter(im);

    // grayscale
    grayscale = filter.grayscale();
    location = target_location(filename, "grayscale.png");
    success = stbi_write_png(location.c_str(), w, h, 1, grayscale, 0);

    // acb
    if(c>1){
        data = filter.acb();
        location = target_location(filename, "acb.png");
        success = stbi_write_png(location.c_str(), w, h, c, data, 0);
    }

    // brightness
    data = filter.brightness(-50);
    location = target_location(filename, "brightness.png");
    success = stbi_write_png(location.c_str(), w, h, c, data, 0);

    // histogram
    if(c>1){
        data = filter.histogram_equalize();
        location = target_location(filename, "histogram.png");
        success = stbi_write_png(location.c_str(), w, h, 1, data, 0);
    }

    // gaussian blur
    data = filter.gaussian_blur(5);
    location = target_location(filename, "gaussian_blur.png");
    success = stbi_write_png(location.c_str(), w, h, c, data, 0);

    // median blur
    data = filter.median_blur_2d(5);
    location = target_location(filename, "median_blur.png");
    success = stbi_write_png(location.c_str(), w, h, c, data, 0);

    // box blur
    data = filter.boxblur(5);
    location = target_location(filename, "box_blur.png");
    success = stbi_write_png(location.c_str(), w, h, c, data, 0);

    // prewitt
    data = filter.prewitt();
    location = target_location(filename, "prewitt.png");
    success = stbi_write_png(location.c_str(), w, h, 1, data, 0);

    // scharr
    Filter filter2(im);
    data = filter2.scharr();
    location = target_location(filename, "scharr.png");
    success = stbi_write_png(location.c_str(), w, h, 1, data, 0);

    // sobel
    Filter filter3(im);
    data = filter3.sobel();
    location = target_location(filename, "sobel.png");
    success = stbi_write_png(location.c_str(), w, h, 1, data, 0);

    // robert
    Filter filter4(im);
    data = filter4.robert();
    location = target_location(filename, "robert.png");
    success = stbi_write_png(location.c_str(), w, h, 1, data, 0);


    auto end = chrono::steady_clock::now();
    cout << "(" << filename << ") Elapsed time in seconds: " << chrono::duration_cast<chrono::microseconds>(end-start).count() << " microsec." << endl;
    stbi_image_free(data);
}

void apply_projections(Volume vol, string dirname, string filename, int minZ=0, int maxZ=-1){
    /*
    Applies Projections
    */

    Projection proj(vol);
    unsigned char* data;
    string folder;
    string location;
    int success;

    // MIP
    cout << "MIP" << endl;
    data = proj.mip(minZ, maxZ);
    folder = "MIP/";
    folder.append(filename);
    location = target_location(dirname, folder);
    success = stbi_write_png(location.c_str(), proj.w, proj.h, 1, data, 0);

    // MINIP
    cout << "MINIP" << endl;
    data = proj.minip(minZ, maxZ);
    folder = "MINIP/";
    folder.append(filename);
    location = target_location(dirname, folder);
    success = stbi_write_png(location.c_str(), proj.w, proj.h, 1, data, 0);

    // AIP
    cout << "AIP" << endl;
    data = proj.aip(false, minZ, maxZ);
    folder = "AIP/";
    folder.append(filename);
    location = target_location(dirname, folder);
    success = stbi_write_png(location.c_str(), proj.w, proj.h, 1, data, 0);

    // MEDIP
    cout << "MEDIP" << endl;
    data = proj.aip(true, minZ, maxZ);
    folder = "MEDIP/";
    folder.append(filename);
    location = target_location(dirname, folder);
    success = stbi_write_png(location.c_str(), proj.w, proj.h, 1, data, 0);
}


void project_scans(const string dirname, int x, int y, int minZ, int maxZ){
    /*
    Preprocesses the scans and calls the projection functions.
    */

    Volume vol(dirname);
    Filter filter(vol);
    Slice slice(vol);
    unsigned char* data;
    int success;
    string location;

    // y-z plane slice
    data = slice.slice(x, 'x');
    location = target_location(dirname, "YZ-slice.png");
    success = stbi_write_png(location.c_str(), vol.h, vol.d, 1, data, 0);

    // x-z plane slice
    data = slice.slice(y, 'y');
    location = target_location(dirname, "XZ-slice.png");
    success = stbi_write_png(location.c_str(), vol.w, vol.d, 1, data, 0);

    // Raw
    cout << "Raw projection" << endl;
    apply_projections(vol, dirname, "raw.png");

    // Slab
    cout << "Slab projection" << endl;
    apply_projections(vol, dirname, "slab.png", minZ, maxZ);

    // 3D Gaussian
    cout << "3D Gaussian" << endl;
    unsigned char** blurred = filter.gaussian_blur_3D(3);
    vol(blurred);
    apply_projections(vol, dirname, "gaussian3D.png");

    // 3D Median
    cout << "3D Median" << endl;
    blurred = filter.median_blur_3D(5);
    vol(blurred);
    apply_projections(vol, dirname, "median3D.png");
}


int main() {
    
    // 2D images
    
    char* path;
    for (const auto & file: std::filesystem::directory_iterator("../Images/"))
        apply_filters(file.path());
    
    
    // 3D scans
    // The Scans folder is not pushed into github due to large size
    // Download the Scans dataset and put inside the program root directory if you want to run for 3D.
    
    project_scans("../Scans/confuciusornis", 400, 420, 10, 70);
    project_scans("../Scans/fracture", 275, 138, 138, 275);
    
    cout << "Finished" << endl;
    return 0;
}