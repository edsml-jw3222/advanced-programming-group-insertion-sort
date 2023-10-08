
#pragma once
#include "Image.h"
#include "../src/group_members.h"
#include <string>

using std::string;

class Volume : public Image
{
public:
    /**
     * @brief Default constrctor for the Volume class.
     */
    Volume(){};

    /**
     * @brief Constructs a Volume object from a directory containing image files.
     * @param dir The directory containing the image files.
     */
    Volume(string dir);


    /**
     * @brief Constructs a Volume object from a data array, dimensions, and channel count.
     * @param data A pointer to the input data array.
     * @param w The width of the volume.
     * @param h The height of the volume.
     * @param d The depth of the volume.
     * @param c The number of channels in the volume.
     */
    Volume(unsigned char** input, int w, int h, int d, int c);

    /**
     * @brief Overloads the () operator for initializing the Volume object's data.
     * @param init A pointer to the data array.
     */
    void operator()(unsigned char** init);
    ~Volume();

    unsigned char** data3D;
    unsigned char* flat;
    int d; // depth

    /**
     * @brief Flattens the data array of the Volume object.
     * @return A pointer to the flattened data array.
     */
    unsigned char* flatten_data();
};


