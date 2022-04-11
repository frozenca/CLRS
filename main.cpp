#include <iostream>
#include <vector>
#include "02_getting_started/inc/easy.h"
#include "02_getting_started/inc/hard.h"

int main() {
    namespace fc = frozenca;
    using namespace std;

    {
        vector<int> v{2, 3, 1, 6, 5, 4};
        fc::selection_sort(v);
        fc::print(v);
    }
    {
        vector<int> v{2, 3, 1, 6, 5, 4};
        fc::hard::selection_sort(v);
        fc::print(v);

        fc::perf_check_sorting(fc::hard::selection_sort);
    }

}