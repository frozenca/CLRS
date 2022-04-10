#include <iostream>
#include <vector>
#include "../easy/01_sum_array_easy.h"

int main() {
    namespace fc = frozenca::easy;
    using namespace std;

    vector<int> v {2, 3, 1, 6, 5, 4};
    cout << fc::sum_array(v);

}