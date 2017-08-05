#ifndef GRADIENT_MAP_H
#define GRADIENT_MAP_H

#include "opencv2/core.hpp"
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"

#include <math.h>

#define NUM_ORIENTATION 16
#define GX 0
#define GY 1

using namespace std;
using namespace cv;

class GradientMap {
public:
  GradientMap(cv::Mat &im);
  ~GradientMap();
  void write_orientation_matrix();
  void write_gradient_maps();
  cv::Mat &get_orientation_matrix();

private:
  void gradients_map(cv::Mat &im);
  void orientation_matrix();
  int get_orientation_value(short int gx, short int gy);
  cv::Mat gx;
  cv::Mat gy;
  cv::Mat oriented_matrix;
};

#endif
