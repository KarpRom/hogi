#ifndef GRADIENT_MAP_H
#define GRADIENT_MAP_H

#include "opencv2/core.hpp"
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include <cmath>

#include <hogi/constants.hpp>

#define NUM_ORIENTATION 16
#define GX 0
#define GY 1

using namespace std;
using namespace cv;

class GradientMap {
public:
  GradientMap(cv::Mat &im, cv::Mat &mask);
  ~GradientMap();
  void write_orientation_matrix();
  void write_gradient_maps();
  cv::Mat &get_orientation_matrix();

private:
  void compute_gradients(cv::Mat &im);
  void orientation_matrix();
  int get_orientation_value(float gx, float gy);
  cv::Mat gx;
  cv::Mat gy;
  cv::Mat oriented_matrix;
  cv::Mat mask;
};

#endif
