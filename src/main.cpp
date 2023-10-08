/*! \mainpage My Personal Index Page
 *
 * \section intro_sec Introduction
 *
 *
 * Made by Group Insertion-sort
 * 
 * Group Members: Jiajia Zheng, Jie Wang, Michael Tsesmelis, Shichao Hu, Suhan Zhao
 *
 * Click on the class names to view what our library can do!
 *
 * 
 */

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../lib/stb_image_write.h"
#include "Image.h"
#include "Filter.h"
#include "Volume.h"
#include "Projection.h"
#include "Slice.h"

int print_intro(int answer){
    cout << "Choose your transformation:\n" << endl;
    
    cout << "1. Image filter" << endl;
    cout << "2. Edge Detection" << endl;
    cout << "3. Scan (volume) filter and projection " << endl;
    cout << "4. Scan (volume) slicing" << endl;
    cout << "Any other key to change images." << endl;

    cout << "\nChoice: ";
    cin >> answer;
    cout << endl;
    system("clear");
    return answer;
}

int print_statement_image(int answer){
    cout << "Choose your transformation:\n" << endl;

    cout << "### Colour correction ###" << endl;
    cout << "1. Grayscale" << endl;
    cout << "2. Automatic Colour Balance" << endl;
    cout << "3. Brightness" << endl;
    cout << "4. Histogram equalisation\n" << endl;

    cout << "### Image blur ###" << endl;
    cout << "5. Median Blur" << endl;
    cout << "6. Box Blur" << endl;
    cout << "7. Gaussian Blur" << endl;

    cout << "\nChoice: ";
    cin >> answer;
    cout << endl;
    return answer;
}

int print_statement_edge_detection(int answer){
    cout << "### Edge detection ###\n" << endl;
    cout << "1. Sobel" << endl;
    cout << "2. Prewitt" << endl;
    cout << "3. Scharr" << endl;
    cout << "4. Robert's Cross" << endl;
    cout << "5. Save image" << endl;
    cout << "Give any other number to close." << endl;

    cout << "\nChoice: ";
    cin >> answer;
    cout << endl;
    return answer;
}

int print_statement_volume(int answer){
    cout << "Choose your transformation before doing a projection:\n" << endl;

    cout << "1. None" << endl;
    cout << "2. Median Blur" << endl;
    cout << "3. Gaussian Blur" << endl;
    cout << "4. Slab" << endl;

    cout << "\nChoice: ";
    cin >> answer;
    cout << endl;
    return answer;
}

int print_statement_projection(int answer){
    cout << "Choose your projection:\n\n" << endl;

    cout << "1. Maximum Intensity Projection (MIP) " << endl;
    cout << "2. Minimum Intensity Projection (MINIP)" << endl;
    cout << "3. Average Intensity Projection (AIP)" << endl;
    cout << "4. Average Intensity Projection using median (MEDIP)" << endl;

    cout << "\nChoice: ";
    cin >> answer;
    cout << endl;
    return answer;
}

void ask_folders(string& filename, string& outputfolder){
    ifstream myfile;
    string folder;

    cout << "\t### Data Augmentation Tool (DAT) ###\n" << endl;

    while(true){
        cout << "What is the full image/folder name you would like to load? E.g. ../Images/car.png.\nLocation: ";
        cin >> filename;
        myfile.open(filename);
        if(myfile || std::filesystem::is_directory(filename) || std::filesystem::exists(filename)) // check if exists
            break;
        else
            cout << "Image or folder doesn't exist, try again..." << endl;
    }

    cout << "Where would you like to save the output(s)? Give it as '{folder}/' \nLocation: ";
    cin >> outputfolder;
    if (!std::filesystem::is_directory(outputfolder) || !std::filesystem::exists(outputfolder)){ // Check if src folder exists
        std::filesystem::create_directory(outputfolder); // create src folder
    }

    system("clear");
}


void run_image(string filename, string outputfolder){
    int answer, w, h, c;
    int minZ = 0;
    int maxZ = -1;
    unsigned char* image;
    unsigned char* data;
    string outputname;

    answer = print_statement_image(answer);

    image = stbi_load(filename.c_str(), &w, &h, &c, 0);
    Image im(image, w, h, c);
    Filter filter(im);

    switch (answer)
    {
        case 1:
            data = filter.grayscale();
            c = 1;
            outputname = "grayscale.png";
            break;

        case 2:
            data = filter.acb();
            outputname = "acb.png";
            break;

        case 3:
            cout << "How much brightness would you like to add or subtract? (0 for average of 128)" << endl;
            cin >> answer;
            data = filter.brightness(answer);
            outputname = "brightness_";
            outputname.append(to_string(answer));
            outputname.append(".png");
            break;

        case 4:
            data = filter.histogram_equalize();
            outputname = "histogram.png";
            c = 1;
            break;

        case 5:
            cout << "What do you want the size of your kernel to be?" << endl;
            cin >> answer;
            data = filter.median_blur_2d(answer);
            outputname = "median_blur_2d.png";
            break;

        case 6:
            cout << "What do you want the size of your kernel to be?" << endl;
            cin >> answer;
            data = filter.boxblur(answer);
            outputname = "boxblur.png";
            break;

        case 7:
            cout << "What do you want the size of your kernel to be?" << endl;
            cin >> answer;
            data = filter.gaussian_blur(answer);
            outputname = "gaussian_blur_2d.png";
            break;
        
        default:
            return;
            break;
    }

    outputfolder.append(outputname);
    int success = stbi_write_png(outputfolder.c_str(), w, h, c, data, 0);
}


void run_detection(string filename, string outputfolder){
    int answer, w, h, c;
    string outputname;
    unsigned char* image;
    unsigned char* data;

    image = stbi_load(filename.c_str(), &w, &h, &c, 0);
    Image im(image, w, h, c);
    Filter filter(im);

    bool continued = true;
    while(continued){
        answer = print_statement_edge_detection(answer);

        switch (answer)
        {
        case 1:
            data = filter.sobel();
            outputname.append("sobel_");
            break;
            
        case 2:
            data = filter.prewitt();
            outputname.append("prewitt_");
            break;

        case 3:
            data = filter.scharr();
            outputname.append("scharr_");
            break;

        case 4:
            data = filter.robert();
            outputname.append("robert_");
            break;

        case 5: 
            if(data){
                    outputname.append(".png");
                    outputfolder.append(outputname);
                    int success = stbi_write_png(outputfolder.c_str(), w, h, 1, data, 0);
            }
            else
                    cout << "Nothing to save" << endl;
            break;
        
        default:
            continued = false;
            break;
        }
    }
}


void run_volume(string filename, string outputfolder){
    int answer;
    int minZ = 0;
    int maxZ = -1;
    string outputname;
    unsigned char* image;
    unsigned char** data;

    answer = print_statement_volume(answer);
 
    Volume vol(filename);
    Filter filter(vol);

    switch (answer)
    {
        case 1:
            outputname = "raw_";
            break;

        case 2:
            cout << "What do you want the size of your kernel to be?" << endl;
            cin >> answer;
            data = filter.median_blur_3D(answer);
            vol(data);
            outputname = "median_blur_3d_";
            break;

        case 3:
            cout << "What do you want the size of your kernel to be?" << endl;
            cin >> answer;
            data = filter.gaussian_blur_3D(answer);
            vol(data);
            outputname = "gaussian_blur_3d_";
            break;

        case 4:
            cout << "At what depth should the slab begin?" << endl;
            cin >> minZ;
            cout << "At what depth should the slab end?" << endl;
            cin >> maxZ;
            outputname = "slab_";
            break;
        
        default:
            return;
            break;
    }

    answer = print_statement_projection(answer);

    Projection proj(vol);
    unsigned char* projection;

    switch (answer)
    {
        case 1:
            projection = proj.mip(minZ, maxZ);
            outputname.append("mip.png");
            break;
        
        case 2:
            projection = proj.minip(minZ, maxZ);
            outputname.append("minip.png");
            break;

        case 3:
            projection = proj.aip(false, minZ, maxZ);
            outputname.append("aip.png");
            break;

        case 4:
            projection = proj.aip(true, minZ, maxZ);
            outputname.append("medip.png");
            break;
        
        default:
            return;
            break;
    }

    outputfolder.append(outputname);
    int success = stbi_write_png(outputfolder.c_str(), proj.w, proj.h, proj.ch, projection, 0);
}

void run_slice(string filename, string outputfolder){
    int answer, w, h;
    string outputname;
    char ax;
    int p0;
    unsigned char* data;

    cout << "What axis would you like to cut?" << endl;
    cin >> ax;
    cout << "At what value would you like to cut it?" << endl;
    cin >> p0;
    Volume vol(filename);
    Slice slice(vol);

    bool run = true;
    while(run){
        if(ax == 'x'){
            data = slice.slice(p0, 'x');
            outputname = "sliceyz_";
            outputname.append(to_string(p0));
            outputname.append(".png");
            w = slice.h;
            h = slice.d;
            break;
        }
        else if(ax == 'y'){
            data = slice.slice(p0, 'y');
            outputname = "slicexz.png";
            outputname.append(to_string(p0));
            outputname.append(".png");
            w = slice.w;
            h = slice.d;
            break;
        }
        else
            cout << "Unknown axis, try again" << endl;
    }
    
    outputfolder.append(outputname);
    int success = stbi_write_png(outputfolder.c_str(), w, h, slice.ch, data, 0);
}

int main(){
    int answer;
    string filename;
    string outputfolder;
    bool continued;


    while(true){ // working on new picture
        ask_folders(filename, outputfolder);

        continued=true;
        while(continued){ // same picture, different filter
            answer = print_intro(answer);

            switch (answer)
            {
                case 1:
                    run_image(filename, outputfolder);
                    break;

                case 2:
                    run_detection(filename, outputfolder);
                    break;

                case 3:
                    run_volume(filename, outputfolder);
                    break;

                case 4:
                    run_slice(filename, outputfolder);
                    break;

                default:
                    continued = false;
                    break;
            }
            system("clear");
            cout << "\n\nFinished transformation, starting new one...\n" << endl;
        }
        system("clear");
        cout << "\n\nChanging images...\n" << endl;
    }
}