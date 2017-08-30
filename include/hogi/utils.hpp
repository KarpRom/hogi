#ifndef UTILS_H
#define UTILS_H

#include "opencv2/core.hpp"
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

inline float square_euclidean_distance(cv::Point p1, cv::Point p2) {
  float dx = p1.x - p2.x;
  float dy = p1.y - p2.y;
  return dx * dx + dy * dy;
}

inline string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch (depth) {
  case CV_8U:
    r = "8U";
    break;
  case CV_8S:
    r = "8S";
    break;
  case CV_16U:
    r = "16U";
    break;
  case CV_16S:
    r = "16S";
    break;
  case CV_32S:
    r = "32S";
    break;
  case CV_32F:
    r = "32F";
    break;
  case CV_64F:
    r = "64F";
    break;
  default:
    r = "User";
    break;
  }

  r += "C";
  r += (chans + '0');

  return r;
}

inline void printMatType(cv::Mat im) {
  string ty = type2str(im.type());
  printf("Matrix: %s %dx%d \n", ty.c_str(), im.cols, im.rows);
}

#endif
