#include "utility.h"
#include "../src/group_members.h"
#include <vector>
#include <string>

using namespace std;

int util::minimum(int a, int b){
        if(a>b)
            return b;
        else
            return a;
}

int util::maximum(int a, int b){
    if(a<b)
        return b;
    else
        return a;
}

vector<string> util::insert(vector<string> files, string path){
    int pos = 0;
    if(files.size()==0)
        files.insert(files.begin(), path);
    else{
        for (auto it = files.begin(); it != files.end(); it++) {
            if(path < *it){
                files.insert(it, path);
                break;
            } else if (it == files.end()-1){
                files.push_back(path);
                break;
            }
        }
    }

    return files;
}

unsigned char util::get_median_odd(unsigned char* values, int size)
{
    int middle = size / 2;
    unsigned char median = 0;
    int histogram[256] = {0}; // histogram of pixel values

    for (int i = 0; i < size; i++) // build histogram
    {
        histogram[values[i]]++;
    }

    int count = 0;
    for (int i = 0; i < 256; i++) // find the median
    {
        count += histogram[i];
        if (count > middle)
        {
            median = i;
            break;
        }
    }

    return median;
}

unsigned char util::get_median(unsigned char* values, int size)
{
    int middle = size / 2 + 1;
    unsigned char median = 0;
    int histogram[256] = {0};  // histogram of pixel values

    for (int i = 0; i < size; i++) // build histogram
    {
        histogram[values[i]]++;  
    }

    int count = 0;
    for (int i = 0; i < 256; i++) // find the median
    {
        count += histogram[i]; 
        if (count >= middle)
        {
            if (size % 2 == 0 && count == middle)  // even number of elements
            {
                // find the next value that occurs in the histogram
                int j = i + 1;
                while (j < 256 && histogram[j] == 0) j++;

                // calculate the average of the two middle values
                median = (i + j) / 2;
            }
            else  // odd number of elements
            {
                median = i;
            }
            break;
        }
    }

    return median;
}