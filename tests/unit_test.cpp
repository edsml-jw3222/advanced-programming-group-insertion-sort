#include "test_filter.h"
#include "test_projections.h"
#include "test_slicing.h"
#include "../src/group_members.h"
#include <chrono>

using namespace TestFilter;
using namespace TestProjection;
using namespace TestSlicing;

void test_filters(){
    // Filters
    test_grayscale();
    test_acb();
    test_brightness();
    test_histogram();

    // Convolutions
    test_gaussian_blur();
    test_median_blur_2d();
    test_box_blur();
    test_gaussian_blur_3d();

    // Edge detections
    test_robert();
    test_sobel();
    test_prewitt();
    test_scharr();
}

void test_projections(){
    test_mip(); 
    test_minip();
    test_aip();
}

void test_slicing(){
    test_slicing_x();
    test_slicing_y();
}

int main() {

    test_filters();

    test_projections();

    test_slicing();

    return 0;
}