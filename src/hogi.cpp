#include <iostream>
#include <hogi/gradient_map.hpp>

int main(int argc, char **argv) {
  cv::Mat im = imread(argv[1], 0);
  GradientMap gm(im);
  gm.write_orientation_matrix();
  gm.write_gradient_maps();
  return 0;
}
