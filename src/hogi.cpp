#include <iostream>
#include <hogi/gradientMap.hpp>

int main(int argc, char **argv) {
  cv::Mat im = imread(argv[1]);
  GradientMap gm(im);
  gm.write_orientation_matrix();
  return 0;
}
