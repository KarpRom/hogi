#include <iostream>
#include <hogi/gradient_map.hpp>
#include <hogi/boundary_extraction.hpp>

int main(int argc, char **argv) {
  cv::Mat im = imread(argv[1], 0);
  cv::Mat mask = imread(argv[2], 0);

  GradientMap gm(im);
  gm.write_orientation_matrix();
  gm.write_gradient_maps();

  BoundaryExtraction boundary_extraction(mask);

  return 0;
}
