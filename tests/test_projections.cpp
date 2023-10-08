#include "test_projections.h"
#include "../src/Projection.h"
#include "../src/group_members.h"
#include <iostream>
#include <cassert>

void TestProjection::test_aip(){
    unsigned char** dataset = new unsigned char*[3];
    dataset[0] = new unsigned char[9]{1, 2, 3, 4, 5, 6, 7, 8, 9};
    dataset[1] = new unsigned char[9]{5, 6, 5, 8, 9, 2, 2, 2, 3};
    dataset[2] = new unsigned char[9]{12, 13, 1, 3, 4, 7, 0, 2, 9};

    Volume vol(dataset, 3, 3, 3, 1);

    Projection proj(vol);
    unsigned char* result_mean = proj.aip(false);
    unsigned char* result_median = proj.aip(true);  

    unsigned char* expected_mean = new unsigned char[9]{6, 7, 3, 5, 6, 5, 3, 4, 7};
    unsigned char* expected_median = new unsigned char[9]{5, 6, 3, 4, 5, 6, 2, 2, 9};

    for(int i = 0; i < 9; i++){
        assert(result_mean[i] == expected_mean[i] && result_median[i] == expected_median[i]);
    }

    delete[] result_mean;
    delete[] result_median;
    std::cout << "AIP passed the test" << std::endl;
}

void TestProjection::test_mip(){
    unsigned char** dataset = new unsigned char*[3];
    dataset[0] = new unsigned char[9]{1, 2, 3, 4, 5, 6, 7, 8, 9};
    dataset[1] = new unsigned char[9]{5, 6, 7, 8, 9, 1, 2, 3, 4};
    dataset[2] = new unsigned char[9]{11, 13, 1, 2, 4, 7, 1, 2, 9};

    Volume vol(dataset, 3, 3, 3, 1);

    Projection proj(vol);
    unsigned char* result = proj.mip();
    unsigned char* result_slab = proj.mip(0,1);  

    unsigned char* expected = new unsigned char[9]{11, 13, 7, 8, 9, 7, 7, 8, 9};
    unsigned char* expected_slab = new unsigned char[9]{5, 6, 7, 8, 9, 6, 7, 8, 9};


    for(int i = 0; i < 9; i++){
        assert(result[i] == expected[i] && result_slab[i] == expected_slab[i]);
    }

    delete[] result;
    delete[] result_slab;
    std::cout << "MIP passed the test" << std::endl;
}

void TestProjection::test_minip(){
    unsigned char** dataset = new unsigned char*[3];
    dataset[0] = new unsigned char[9]{1, 2, 3, 4, 5, 6, 7, 8, 9};
    dataset[1] = new unsigned char[9]{5, 6, 7, 8, 9, 1, 2, 3, 4};
    dataset[2] = new unsigned char[9]{11, 13, 1, 2, 4, 7, 1, 2, 9};

    Volume vol(dataset, 3, 3, 3, 1);

    Projection proj(vol);
    unsigned char* result = proj.minip();

    unsigned char expected[] = {1, 2, 1, 2, 4, 1, 1, 2, 4};


    for(int i = 0; i < 9; i++){
        assert(result[i] == expected[i]);
    }

    delete[] result;
    std::cout << "MinIP passed the test" << std::endl;
}
