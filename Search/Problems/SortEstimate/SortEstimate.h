#ifndef ALGO_AUTOLOAN_H
#define ALGO_AUTOLOAN_H

#include <cmath>
#include <exception>

namespace SortEstimate {

bool is_essentially_equal(const double& a, const double& b, const double& tolerance = 1e-9) {
  double diff = std::fabs(a - b);
  return (std::fabs(a - b) <= tolerance);
}


/**
 *
 * @param constant   - algo's constant
 * @param numbers    - number of integers to process
 * @return  double - time taken to process n integers
 */
double get_processing_time(const int constant, const double& numbers) {
  return ( static_cast<double>(constant) * numbers * std::log(numbers)/std::log(2) );
}

/**
 * @brief       determines how many integers can be processed for for a given algorithm having O(n): c*n*lg(n)
 * @param c     constant of the underlying algo's O(n)
 *                  Range: 1 - 100
 * @param time  time taken to process "n" integers by the algorithm
 *                  Range: 1 - 2000000000
 * @return      double -> number of integers that can be processed
 */
double how_many(int c, int time, double tolerance = 1e-9) {

  double numbers{1.00};   // current number of integers being processed
  double high_num{2000000000};  // max number of integers that can be processed
  double mid;
  double processing_time;

  while(!is_essentially_equal(numbers, high_num, tolerance)) {
    mid = numbers + ((high_num - numbers)/2);
    processing_time = get_processing_time(c, mid);
    if(processing_time < time) {
      numbers = mid;  // can process more integers - we still have some leeway
    } else {
      high_num = mid;
    }
  }

  return numbers;


}

} // namespace SortEstimate

#endif //ALGO_AUTOLOAN_H