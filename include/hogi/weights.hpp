#include "opencv2/core.hpp"
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include <cmath>

#include <hogi/constants.hpp>

using namespace cv;

const Mat w1 = (Mat_<double>(5, 5) << 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0,
                0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0);
const Mat w2 = (Mat_<double>(5, 5) << 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,
                0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0);
const Mat w3 = (Mat_<double>(5, 5) << 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,
                0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1);
const Mat w4 = (Mat_<double>(5, 5) << 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0,
                0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1);
const Mat w5 = (Mat_<double>(5, 5) << 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0,
                0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1);
const Mat w6 = (Mat_<double>(5, 5) << 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0,
                0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0);
const Mat w7 = (Mat_<double>(5, 5) << 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1,
                1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0);
const Mat w8 = (Mat_<double>(5, 5) << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
                1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
const Mat w9 = (Mat_<double>(5, 5) << 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
                1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0);
const Mat w10 = (Mat_<double>(5, 5) << 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0,
                 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0);
const Mat w11 = (Mat_<double>(5, 5) << 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0,
                 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0);
const Mat w12 = (Mat_<double>(5, 5) << 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0,
                 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0);
const Mat w13 = (Mat_<double>(5, 5) << 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0,
                 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0);
const Mat w14 = (Mat_<double>(5, 5) << 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0,
                 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0);
const Mat w15 = (Mat_<double>(5, 5) << 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0,
                 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0);
const Mat w16 = (Mat_<double>(5, 5) << 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0,
                 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0);

inline cv::Mat get_weight_matrix_0() {
  cv::Mat w0(5, 5, CV_32FC1);
  for (int x = 0; x < 5; x++)
    for (int y = 0; y < 5; y++)
      w0.at<float>(y, x) = exp(-(pow(x - 3, 2) + pow(y - 3, 2)) / 50);
  return w0;
}

inline cv::Mat get_weight_matrix(int x) {
  assert(x >= 0 && x <= 16);
  switch (x) {
  case 1:
    return w1;
  case 2:
    return w2;
  case 3:
    return w3;
  case 4:
    return w4;
  case 5:
    return w5;
  case 6:
    return w6;
  case 7:
    return w7;
  case 8:
    return w8;
  case 9:
    return w9;
  case 10:
    return w10;
  case 11:
    return w11;
  case 12:
    return w12;
  case 13:
    return w13;
  case 14:
    return w14;
  case 15:
    return w15;
  case 16:
    return w16;
  default:
    return get_weight_matrix_0();
  }
}
