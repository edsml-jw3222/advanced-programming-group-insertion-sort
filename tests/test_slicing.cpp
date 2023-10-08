#include "../src/group_members.h"
#include "../src/Slice.h"
#include "test_slicing.h"
#include <cassert>
#include <iostream>

void TestSlicing::test_slicing_x(){
    unsigned char** data = new unsigned char*[3];
    data[0] = new unsigned char[9]{1, 2, 3, 4, 5, 6, 7, 8, 9};
    data[1] = new unsigned char[9]{5, 6, 5, 8, 9, 2, 2, 2, 3};
    data[2] = new unsigned char[9]{12, 13, 1, 3, 4, 7, 0, 2, 9};

    Volume vol(data, 3, 3, 3, 1);

    Slice slicing(vol);
    unsigned char* x_slice_plane = slicing.slice(1, 'x');

    unsigned char* expected_slice_plane = new unsigned char[9]{2, 5, 8, 6, 9, 2, 13, 4, 2};

    for(int i = 0; i < 9; i++){
        assert(x_slice_plane[i] == expected_slice_plane[i]);
    }

    delete[] expected_slice_plane;
    delete[] x_slice_plane;

    std::cout << "Slicing x test passed" << std::endl;

}

void TestSlicing::test_slicing_y(){
    unsigned char** data = new unsigned char*[3];
    data[0] = new unsigned char[9]{1, 2, 3, 4, 5, 6, 7, 8, 9};
    data[1] = new unsigned char[9]{5, 6, 5, 8, 9, 2, 2, 2, 3};
    data[2] = new unsigned char[9]{12, 13, 1, 3, 4, 7, 0, 2, 9};

    Volume vol(data, 3, 3, 3, 1);

    Slice slicing(vol);

    unsigned char* y_slice_plane = slicing.slice(2, 'y');

    unsigned char* expected_slice_plane = new unsigned char[9]{7, 8, 9, 2, 2, 3, 0, 2, 9};

    for(int i = 0; i < 9; i++){
        assert(y_slice_plane[i] == expected_slice_plane[i]);
    }

    delete[] expected_slice_plane;
    delete[] y_slice_plane;

    std::cout << "Slicing y test passed" << std::endl;
}