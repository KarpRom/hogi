#ifndef DOMINANT_GRADIENT_H
#define DOMINANT_GRADIENT_H

#include "opencv2/core.hpp"
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include <cmath>
#include <iostream>

#include <hogi/constants.hpp>
#include <hogi/utils.hpp>
#include <hogi/types.hpp>

using namespace cv;
using namespace std;

#define HISTOGRAM_SIZE 16

class DominantGradient {
public:
  DominantGradient() {}
  ~DominantGradient() {}

  int get_dominant_gradient(cv::Mat &patch, cv::Mat &mask_patch, cv::Point p,
                            cv::Point center, Contour &c);
  void init_histogram();
  void compute_histogram(cv::Mat &patch, cv::Mat &mask, cv::Point p,
                         cv::Point center, Contour &c);
  float get_histogram_sum();
  void normalize_histogram();
  int get_max_value();
  void print_histogram();

private:
  std::vector<float> histogram;
  int it = 0;
};

#endif
