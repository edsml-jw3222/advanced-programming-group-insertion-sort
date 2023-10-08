
#pragma once
#include "Volume.h"
#include "../src/group_members.h"

/**
 * @class Slice
 * @brief A class derived from the Volume class, responsible for extracting a 2D slice from the volume data.
 */
class Slice: public Volume
{
public:

    /**
     * @brief Constructs a Slice object using a reference to a Volume object.
     * 
     * @param vol A reference to a Volume object.
     */
    Slice(Volume& vol);
    ~Slice();


    /**
     * @brief Extracts a 2D slice from the volume data.
     * 
     * @param p0 The index of the slice along the specified fixed coordinate.
     * @param fixed_coor The fixed coordinate along which the slice is extracted. 'x' for slicing along the X-axis, and 'y' for slicing along the Y-axis.
     * @return A pointer to the output slice image data.
     */
    unsigned char* slice(int p0, char fixed_coor);

private:
    Volume& volume;
};
