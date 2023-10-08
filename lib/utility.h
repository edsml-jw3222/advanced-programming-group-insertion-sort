#include "../src/group_members.h"
#include <vector>
#include <string>

using namespace std;

namespace util
{
    /**
     * @brief Returns the minimum of two integers.
     * @param a The first integer.
     * @param b The second integer.
     * @return The minimum of a and b.
     */
    int minimum(int a, int b);

    /**
     * @brief Returns the maximum of two integers.
     * @param a The first integer.
     * @param b The second integer.
     * @return The maximum of a and b.
     */
    int maximum(int a, int b);

    /**
     * @brief Inserts a path string into a vector of strings in sorted order.
     * @param files The input vector of strings.
     * @param path The string to be inserted.
     * @return The modified vector with the inserted path string.
     */
    vector<string> insert(vector<string> files, string path);

    /**
     * @brief Calculates the median of an array of unsigned chars.
     * @param values The input array of unsigned chars.
     * @param size The size of the input array.
     * @return The median value as an unsigned char.
     */
    unsigned char get_median(unsigned char* values, int size);

    /**
     * @brief Calculates the median of an array of unsigned chars when we are sure the array is of odd length.
     * @param values The input array of unsigned chars.
     * @param size The size of the input array.
     * @return The median value as an unsigned char.
     */
    unsigned char get_median_odd(unsigned char* values, int size);
}
