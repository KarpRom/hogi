#include "hogi/boundary_extraction.hpp"

BoundaryExtraction::BoundaryExtraction(cv::Mat &mask) {
  this->mask = mask;
  this->extractBoundary();
}

BoundaryExtraction::~BoundaryExtraction() {}

pair<Contour, Contour> BoundaryExtraction::extractBoundary() {
  // structuring element of size 3x3
  cv::Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));

  // Outer boundary == dilation - original
  Mat dilated;
  Mat eroded;
  Mat outer_boundary;
  Mat inner_boundary;

  cv::erode(this->mask, dilated, element);
  subtract(this->mask, dilated, outer_boundary);

  // Inner boundary == original - erosion
  cv::dilate(this->mask, eroded, element);
  subtract(eroded, this->mask, inner_boundary);

  cv::imwrite("out/mask.png", this->mask);
  cv::imwrite("out/dilated.png", dilated);
  cv::imwrite("out/eroded.png", eroded);
  cv::imwrite("out/outer_boundary.png", outer_boundary);
  cv::imwrite("out/inner_boundary.png", inner_boundary);

  return mat_boundary_to_list(outer_boundary, inner_boundary);
}

pair<Contour, Contour>
BoundaryExtraction::mat_boundary_to_list(cv::Mat &outer_boundary,
                                         cv::Mat &inner_boundary) {
  Contour inner_contour, outer_contour;
  for (size_t row = 0; row < outer_boundary.rows; row++)
    for (size_t col = 0; col < outer_boundary.cols; col++) {
      if (outer_boundary.at<uchar>(row, col) == 255)
        outer_contour.push_back(Point(col, row));
      if (inner_boundary.at<uchar>(row, col) == 255)
        inner_contour.push_back(Point(col, row));
    }
}

void BoundaryExtraction::setContour(Contour &c) {
  for (size_t i = 0; i < c.size(); i++)
    this->mask.at<uchar>(c[i].y, c[i].x) = 255;
}
