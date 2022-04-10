#include <iostream>
#include <vector>
#include "../inc/easy.h"
#include "../inc/hard.h"

int main() {
    namespace fc = frozenca;
    using namespace std;

    {
        vector<int> v{2, 3, 1, 6, 5, 4};
        cout << fc::easy::sum_array(v) << '\n';
    }
    {
        vector<int> v{2, 3, 1, 6, 5, 4};
        cout << fc::sum_array(v) << '\n';
    }

}