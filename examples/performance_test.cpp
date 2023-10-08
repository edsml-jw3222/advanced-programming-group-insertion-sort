/*
This code was used to measure the performance of our functions. The results are in the report.
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

void test_2d(){
    int w, h, c;
    unsigned char* data;
    unsigned char* image;
    int success;
    string name;
    string imagename[6] = {"../Images/dimorphos.png",
                            "../Images/gracehopper.png",
                            "../Images/stinkbug.png",
                            "../Images/tienshan.png",
                            "../Images/vh_anatomy.png",
                            "../Images/vh_ct.png"};

    for(int k=0; k<6;k++){
        name = imagename[k];
        // Read in image file
        image = stbi_load(name.c_str(), &w, &h, &c, 0);
        unsigned char* grayscale;
        cout << ">>>>> " << name << " (" << w << ", " << h << ")" << endl;
        Image im(image, w, h, c);
        Filter filter(im);

        auto start = chrono::steady_clock::now();
        filter.grayscale();
        auto end = chrono::steady_clock::now();
        cout << "Grayscale: " << chrono::duration_cast<chrono::microseconds>(end-start).count() << "ms" << endl;
        for(int i=3;i<10;i+=2){
            auto start = chrono::steady_clock::now();
            data = filter.gaussian_blur(i);
            auto end = chrono::steady_clock::now();
            cout << "Gaussian Blur 2D with kernel " << i  << ": " << chrono::duration_cast<chrono::microseconds>(end-start).count() << "ms" << endl;

            start = chrono::steady_clock::now();
            data = filter.median_blur_2d(i);
            end = chrono::steady_clock::now();
            cout << "Median Blur 2D with kernel " << i  << ": " << chrono::duration_cast<chrono::microseconds>(end-start).count() << "ms" << endl;
        }
    }
}

void test_3d(){

    string folder50 = "../Scans/confuciusornis50";
    string folder100 = "../Scans/confuciusornis100";
    string folder150 = "../Scans/confuciusornis150";
    string folder200 = "../Scans/confuciusornis200";
    string folder250 = "../Scans/confuciusornis250";

    string folders[5] = {folder50, folder100, folder150, folder200, folder250};
    unsigned char* projection;
    unsigned char** projection3d;


    for(int i=0;i<5;i++){
        cout << folders[i] << endl;
        Volume vol(folders[i]);
        Filter filterv(vol);
        Projection proj(vol);

        auto start = chrono::steady_clock::now();
        proj.mip();
        auto end = chrono::steady_clock::now();
        cout << "MIP: " << chrono::duration_cast<chrono::microseconds>(end-start).count() << "ms" << endl;

        start = chrono::steady_clock::now();
        filterv.median_blur_3D(3);
        end = chrono::steady_clock::now();
        cout << "Median blur (fixed kernel 3): " << chrono::duration_cast<chrono::microseconds>(end-start).count() << "ms" << endl;

        if(i==1){
            for(int j=3;j<9;j+=2){
                start = chrono::steady_clock::now();
                filterv.gaussian_blur_3D(j);
                end = chrono::steady_clock::now();
                cout << "Gaussian blur, kernel " << j << ": " << chrono::duration_cast<chrono::microseconds>(end-start).count() << "ms" << endl;
            }
        }
    }
}


int main(){

    test_2d();

    test_3d();

    return 0;

}