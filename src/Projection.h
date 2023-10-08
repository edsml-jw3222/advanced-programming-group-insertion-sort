#pragma once
#include "Volume.h"
#include "../src/group_members.h"

/**
 * @class Projection
 * @brief A class derived from the Volume class, responsible for applying intensity projection methods to the volume data.
 */
class Projection: public Volume
{
public:
    /**
     * @brief Constructs a Projection object using a reference to a Volume object.
     * 
     * @param vol A reference to a Volume object.
     */
    Projection(const Volume& vol);
    ~Projection();

    /**
     * @brief Computes the Maximum Intensity Projection (MIP) of the 3D data volume.
     *
     * @param minZ The minimum Z index to consider for the projection (default: 0).
     * @param maxZ The maximum Z index to consider for the projection (default: -1, meaning the last index).
     * @return unsigned char* Pointer to the resulting MIP image.
     */
    unsigned char* mip(int minZ = 0, int maxZ = -1);

    /**
     * @brief Computes the Minimum Intensity Projection (MinIP) of the volume.
     *
     * @param minZ The minimum Z index to consider for the projection (default: 0).
     * @param maxZ The maximum Z index to consider for the projection (default: -1, meaning the last index).
     * @return unsigned char* Pointer to the resulting MinIP image.
     */
    unsigned char* minip(int minZ = 0, int maxZ = -1);

    /**
     * @brief Computes the Average Intensity Projection (AIP) of the volume, either using the median or the mean.
     *
     * @param use_median If true, the projection will apply based on the median value for each pixel, otherwise the mean value will be used
     * @param minZ The minimum Z index to consider for the projection (default: 0).
     * @param maxZ The maximum Z index to consider for the projection (default: -1, meaning the last index).
     * @return unsigned char* Pointer to the resulting AIP image.
     */
    unsigned char* aip(bool use_median = false, int minZ = 0, int maxZ = -1);

protected:
    unsigned char* flat;

    /**
     * @brief Ensures that the Z indices for the projection are within the bounds of the volume.
     * 
     * @param minZ The starting Z index for the projection.
     * @param maxZ The ending Z index for the projection.
     */
    void Slab(int& minZ, int& maxZ);
};

