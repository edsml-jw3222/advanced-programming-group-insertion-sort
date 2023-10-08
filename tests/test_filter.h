#include "../src/group_members.h"

namespace TestFilter
{
    // Main filters
    void test_grayscale();
    void test_acb();
    void test_brightness();
    void test_histogram();

    // Convolutions
    void test_gaussian_blur();
    void test_median_blur_2d();
    void test_box_blur();
    void test_gaussian_blur_3d();
    void test_median_blur_3d();

    // Edge detections
    void test_robert();
    void test_sobel();
    void test_prewitt();
    void test_scharr();
};