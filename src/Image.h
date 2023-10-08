
#pragma once
#include "../src/group_members.h"

/**
 * @class Image
 * @brief A class representing a 2D image.
 */
class Image
{
public:

    /**
     * @brief Constructs an empty Image object.
     */
    Image();

    /**
     * @brief Constructs an Image object with the given parameters.
     * 
     * @param data A pointer to the image data.
     * @param w The width of the image.
     * @param h The height of the image.
     * @param c The number of channels in the image.
     */
    Image(unsigned char* data, int w, int h, int c);
    ~Image();

    int w;     // Width of the image
    int h;     // Height of the image
    int ch;    // Number of channels in the image
    int size;  // Total size of the image data (width * height * channels)
    unsigned char* data; // Pointer to the image data
};