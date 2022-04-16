#include <04_divide-and-conquer/inc/easy.h>
#include <04_divide-and-conquer/inc/hard.h>
#include <iostream>
#include <vector>

int main() {
  namespace fc = frozenca;
  using namespace std;

  {
    fc::Matrix<int> A{
        {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    fc::Matrix<int> B{
        {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    {
      auto C = A * B;
      cout << C << '\n';
    }
    {
      auto C = fc::strassen(A, B);
      cout << C << '\n';
    }
  }

  mt19937 gen(random_device{}());
  uniform_int_distribution<size_t> dim_dist(1u, 6u);

  constexpr int num_trials = 10'000;

  auto log2N = dim_dist(gen);
  auto N = (1u << log2N);
  {
    chrono::duration<double, micro> curr_length_duration(0);
    for (int i = 0; i < num_trials; ++i) {
      fc::Matrix<float> A(N, N);
      fc::Matrix<float> B(N, N);
      A.random_fill();
      B.random_fill();
      auto start = chrono::steady_clock::now();
      auto C = A * B;
      auto end = chrono::steady_clock::now();
      curr_length_duration += (end - start);
    }
    fc::log("Time to process matmul of {} x {} and {} x {} : {:10.4f} us\n",
            fc::log_level::all, N, N, N, N,
            (curr_length_duration.count() / num_trials));
  }
  {
    chrono::duration<double, micro> curr_length_duration(0);
    for (int i = 0; i < num_trials; ++i) {
      fc::Matrix<float> A(N, N);
      fc::Matrix<float> B(N, N);
      A.random_fill();
      B.random_fill();
      auto start = chrono::steady_clock::now();
      auto C = fc::strassen(A, B);
      auto end = chrono::steady_clock::now();
      curr_length_duration += (end - start);
    }
    fc::log("Time to process matmul of {} x {} and {} x {} : {:10.4f} us\n",
            fc::log_level::all, N, N, N, N,
            (curr_length_duration.count() / num_trials));
  }
}